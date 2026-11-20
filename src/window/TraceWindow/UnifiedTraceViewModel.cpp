#include "UnifiedTraceViewModel.h"
#include "core/BusTrace.h"
#include "core/Backend.h"
#include "db/model/LinFrame.h"
#include "db/model/CanDbMessage.h"
#include <QColor>
#include <QDateTime>
#include <QLocale>
#include "core/ThemeManager.h"

UnifiedTraceViewModel::UnifiedTraceViewModel(Backend &backend)
    : BaseTraceViewModel(backend)
{
    m_rootItem = std::make_shared<UnifiedTraceItem>(BusMessage()); // Dummy root
    m_firstTimestamp = 0;
    m_previousRowTimestamp = 0;
    m_globalIndexCounter = 1;

    m_processTimer.setInterval(150);
    m_processTimer.setSingleShot(true);
    connect(&m_processTimer, &QTimer::timeout, this, &UnifiedTraceViewModel::processNewMessages);

    connect(backend.getTrace(), &BusTrace::afterAppend, this, &UnifiedTraceViewModel::onTraceDirty);
    connect(backend.getTrace(), &BusTrace::beforeRemove, this, &UnifiedTraceViewModel::beforeRemove);
    connect(backend.getTrace(), &BusTrace::afterRemove, this, &UnifiedTraceViewModel::afterRemove);
    connect(backend.getTrace(), &BusTrace::beforeClear, this, &UnifiedTraceViewModel::beforeClear);
    connect(backend.getTrace(), &BusTrace::afterClear, this, &UnifiedTraceViewModel::afterClear);
    connect(&backend, &Backend::onSetupChanged, this, &UnifiedTraceViewModel::onSetupChanged);
}

UnifiedTraceViewModel::~UnifiedTraceViewModel()
{
}

QModelIndex UnifiedTraceViewModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    UnifiedTraceItem *parentItem;
    if (!parent.isValid())
        parentItem = m_rootItem.get();
    else
        parentItem = static_cast<UnifiedTraceItem*>(parent.internalPointer());

    std::shared_ptr<UnifiedTraceItem> childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem.get());
    else
        return QModelIndex();
}

QModelIndex UnifiedTraceViewModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    UnifiedTraceItem *childItem = static_cast<UnifiedTraceItem*>(child.internalPointer());
    UnifiedTraceItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem.get())
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int UnifiedTraceViewModel::rowCount(const QModelIndex &parent) const
{
    UnifiedTraceItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem.get();
    else
        parentItem = static_cast<UnifiedTraceItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int UnifiedTraceViewModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return BaseTraceViewModel::column_count;
}

bool UnifiedTraceViewModel::hasChildren(const QModelIndex &parent) const
{
    return rowCount(parent) > 0;
}

BusMessage UnifiedTraceViewModel::getMessage(const QModelIndex &index) const
{
    if (!index.isValid()) return BusMessage();
    UnifiedTraceItem *item = static_cast<UnifiedTraceItem*>(index.internalPointer());
    if (item->isSignal()) {
        UnifiedTraceItem *parent = item->parentItem();
        return parent ? parent->rawFrame() : BusMessage();
    }
    return item->rawFrame();
}

QVariant UnifiedTraceViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case Qt::DisplayRole:
            return data_DisplayRole(index, role);
        case Qt::ForegroundRole:
            return data_TextColorRole(index, role);
        case Qt::TextAlignmentRole:
            return BaseTraceViewModel::data_TextAlignmentRole(index, role);
        case ChangedBytesRole:
        {
            if (index.column() != column_data) { return QVariant(); }
            UnifiedTraceItem *item = static_cast<UnifiedTraceItem*>(index.internalPointer());
            if (!item || item->isSignal()) { return QVariant(); }
            const BusMessage &cur  = item->rawFrame();
            const BusMessage &prev = item->prevSameIdFrame();
            if (prev.getLength() == 0) { return QVariant(); }
            if (cur.isErrorFrame() || prev.isErrorFrame()) { return QVariant(); } // text is error flags, not bytes
            uint64_t mask = 0;
            const int len = qMin(cur.getLength(), prev.getLength());
            for (int i = 0; i < len; ++i) {
                if (cur.getData()[i] != prev.getData()[i])
                    mask |= (1ULL << i);
            }
            for (int i = len; i < cur.getLength(); ++i)
                mask |= (1ULL << i);
            return QVariant::fromValue(mask);
        }
        default:
            return BaseTraceViewModel::data(index, role);
    }
}

void UnifiedTraceViewModel::onTraceDirty()
{
    if (!m_processTimer.isActive()) {
        m_processTimer.start();
    }
}

void UnifiedTraceViewModel::processNewMessages()
{
    BusTrace *trace = backend()->getTrace();
    int size = trace->size();
    if (m_lastProcessedIndex >= size) {
        m_lastProcessedIndex = size - 1;
    }

    QList<std::shared_ptr<UnifiedTraceItem>> newItems;

    for (int i = m_lastProcessedIndex + 1; i < size; ++i) {
        const BusMessage msg = trace->getMessage(i);

        auto item = std::make_shared<UnifiedTraceItem>(msg, m_rootItem.get());
        const uint64_t ts = static_cast<uint64_t>(msg.getFloatTimestamp() * 1000000.0);
        if (m_firstTimestamp == 0) m_firstTimestamp = ts;
        item->setTimestamp(ts);
        item->setGlobalIndex(m_globalIndexCounter++);

        const uint64_t dkey = makeDeltaKey(msg);
        item->setPrevSameIdTimestamp(m_prevTimestampByKey.value(dkey, 0));
        m_prevTimestampByKey[dkey] = ts;

        item->setPrevSameIdFrame(m_prevMessageByKey.value(dkey));
        m_prevMessageByKey[dkey] = msg;

        if (msg.busType() == BusType::LIN) {
            LinFrame *linFrame = backend()->findLinFrame(msg);
            if (linFrame) {
                for (int s = 0; s < linFrame->signalList().size(); ++s)
                    item->appendChild(std::make_shared<UnifiedTraceItem>(s, item.get()));
            }
        } else {
            CanDbMessage *dbmsg = backend()->findDbMessage(msg);
            if (dbmsg) {
                for (int s = 0; s < dbmsg->getSignals().length(); ++s)
                    item->appendChild(std::make_shared<UnifiedTraceItem>(s, item.get()));
            }
        }

        newItems.append(item);
        m_lastProcessedIndex = i;
    }

    if (!newItems.isEmpty()) {
        beginInsertRows(QModelIndex(), m_rootItem->childCount(), m_rootItem->childCount() + newItems.size() - 1);
        for (auto &item : newItems) {
            m_rootItem->appendChild(item);
        }
        endInsertRows();
    }

    // Hard row limit check for the UI model
    if (m_rootItem->childCount() > m_maxRows) {
        int toRemove = m_maxRows / 5; // Remove 20% to avoid frequent trimming
        if (toRemove > 0) {
            beginRemoveRows(QModelIndex(), 0, toRemove - 1);
            m_rootItem->removeChildren(0, toRemove);

            // Update row indices for remaining children
            for (int i = 0; i < m_rootItem->childCount(); ++i) {
                m_rootItem->child(i)->setRow(i);
            }
            endRemoveRows();
        }
    }
}

void UnifiedTraceViewModel::beforeRemove(int count)
{
    // Adjust lastProcessedIndex since BusTrace removed items from the front
    m_lastProcessedIndex -= count;
    if (m_lastProcessedIndex < -1) m_lastProcessedIndex = -1;
}

void UnifiedTraceViewModel::afterRemove(int count)
{
    Q_UNUSED(count);
}

void UnifiedTraceViewModel::beforeClear()
{
    beginResetModel();
}

void UnifiedTraceViewModel::afterClear()
{
    m_processTimer.stop();
    m_rootItem = std::make_shared<UnifiedTraceItem>(BusMessage());
    m_lastProcessedIndex = -1;
    m_globalIndexCounter = 1;
    m_firstTimestamp = 0;
    m_previousRowTimestamp = 0;
    m_prevTimestampByKey.clear();
    m_prevMessageByKey.clear();
    endResetModel();
}

void UnifiedTraceViewModel::onSetupChanged()
{
    m_processTimer.stop();
    beginResetModel();
    m_rootItem = std::make_shared<UnifiedTraceItem>(BusMessage());
    m_lastProcessedIndex = -1;
    m_globalIndexCounter = 1;
    m_firstTimestamp = 0;
    m_previousRowTimestamp = 0;
    m_prevTimestampByKey.clear();
    m_prevMessageByKey.clear();

    // Re-process all frames from the trace buffer, bypassing beginInsertRows
    // because the model is already inside beginResetModel/endResetModel.
    processNewMessages();

    endResetModel();
}

uint64_t UnifiedTraceViewModel::makeDeltaKey(const BusMessage &msg)
{
    // Bits 33+: interface ID; bit 32: RX flag; bit 31: extended flag;
    // bits 29-30: bus type; bits 0-28: frame ID
    return static_cast<uint64_t>(msg.getInterfaceId()) << 33
         | static_cast<uint64_t>(msg.isRX()) << 32
         | static_cast<uint64_t>(msg.isExtended()) << 31
         | static_cast<uint64_t>(static_cast<uint8_t>(msg.busType())) << 29
         | (static_cast<uint64_t>(msg.getRawId()) & 0x1FFFFFFFull);
}

QVariant UnifiedTraceViewModel::data_DisplayRole(const QModelIndex &index, [[maybe_unused]] int role) const
{
    UnifiedTraceItem *item = static_cast<UnifiedTraceItem*>(index.internalPointer());

    const uint64_t current = item->timestamp();
    if (item->isSignal()) {
        UnifiedTraceItem *parent = item->parentItem();
        if (!parent) return QVariant();
        if (index.column() == column_index) return QVariant();
        return data_DisplayRole_Signal(index, role, parent->rawFrame());
    }

    const BusMessage& msg = item->rawFrame();
    switch (index.column()) {
        case column_index:
            return (item->parentItem() == m_rootItem.get()) ? QVariant(item->globalIndex()) : QVariant();
        case column_timestamp:
        {
            const uint64_t prev = (item->parentItem() == m_rootItem.get())
                                      ? item->prevSameIdTimestamp()
                                      : item->parentItem()->timestamp();
            return formatUnifiedTimestamp(current, prev);
        }
        case column_channel: return interfaceName(msg.getInterfaceId());
        case column_direction: return msg.isRX() ? tr("RX") : tr("TX");
        case column_type: {
            if (msg.busType() == BusType::LIN) {
                if (msg.isLinSleepFrame())  return QStringLiteral("LIN.SLP");
                if (msg.isLinWakeupFrame()) return QStringLiteral("LIN.WUP");
                return QStringLiteral("LIN");
            }
            if (msg.isErrorFrame()) return QStringLiteral("ERR");
            QString t;
            if (msg.isFD())       t += QStringLiteral("FD.");
            if (msg.isExtended()) t += QStringLiteral("EXT"); else t += QStringLiteral("STD");
            if (msg.isRTR())      t += QStringLiteral(".RTR");
            if (msg.isBRS())      t += QStringLiteral(".BRS");
            if (msg.isESI())      t += QStringLiteral(".ESI");
            return t;
        }
        case column_canid:
            // A CAN error frame carries error classes instead of an ID; a LIN
            // frame flagged as an error is a real frame, so keep its ID.
            if (msg.isErrorFrame() && msg.busType() == BusType::CAN)
                return QStringLiteral("-");
            return QString("0x%1").arg(msg.getId(), 0, 16).toUpper();
        case column_dlc: return msg.getLength();
        case column_data: return dataAsciiMode() ? msg.getDataAsciiString() : msg.getDataHexString();
        case column_name:
            if (msg.busType() == BusType::LIN) {
                if (msg.isLinSleepFrame())  return tr("Sleep");
                if (msg.isLinWakeupFrame()) return tr("Wakeup");
                LinFrame *linFrame = backend()->findLinFrame(msg);
                return linFrame ? linFrame->name() : QStringLiteral("");
            }
            {
                CanDbMessage *dbmsg = backend()->findDbMessage(msg);
                return (dbmsg) ? dbmsg->getName() : tr("[raw]");
            }
        case column_comment: {
                CanDbMessage *dbmsg = backend()->findDbMessage(msg);
                return (dbmsg) ? dbmsg->getComment() : "";
            }
        case column_sender: {
            if (msg.busType() == BusType::LIN) {
                LinFrame *linFrame = backend()->findLinFrame(msg);
                return linFrame ? linFrame->publisher() : QStringLiteral("");
            }
            return "";
        }
        default: return QVariant();
    }
}

QVariant UnifiedTraceViewModel::data_TextColorRole(const QModelIndex &index, [[maybe_unused]] int role) const
{
    UnifiedTraceItem *item = static_cast<UnifiedTraceItem*>(index.internalPointer());
    const bool isDark = ThemeManager::instance().isDarkMode();

    const BusMessage& msg = item->rawFrame();
    if (msg.isErrorFrame()) return isDark ? QColor(255, 100, 100) : QColor(Qt::red);
    return QVariant();
}

QString UnifiedTraceViewModel::formatUnifiedTimestamp(uint64_t ts, uint64_t prevTs) const
{
    double val = 0;
    switch (timestampMode()) {
        case timestamp_mode_absolute:
            return QLocale::c().toString(
                QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(ts / 1000), Qt::LocalTime),
                "hh:mm:ss.zzz");
        case timestamp_mode_absolute_utc:
            return QLocale::c().toString(
                QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(ts / 1000), Qt::UTC),
                "hh:mm:ss.zzz");
        case timestamp_mode_relative:
        {
            const uint64_t startTs = static_cast<uint64_t>(backend()->getTimestampAtMeasurementStart() * 1000000.0);
            val = (ts >= startTs) ? static_cast<double>(ts - startTs) / 1000000.0 : 0.0;
            break;
        }
        case timestamp_mode_delta:
            val = (prevTs > 0 && ts >= prevTs) ? static_cast<double>(ts - prevTs) / 1000000.0 : 0.0;
            break;
        default:
            return QStringLiteral("0.000");
    }
    return QString::number(val, 'f', 3);
}
