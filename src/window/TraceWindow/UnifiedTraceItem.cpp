#include "UnifiedTraceItem.h"

UnifiedTraceItem::UnifiedTraceItem(const BusMessage& frame, UnifiedTraceItem* parent)
    : m_parentItem(parent), m_rawFrame(frame), m_globalIndex(0), m_row(-1)
{
    m_timestamp = static_cast<uint64_t>(frame.getFloatTimestamp() * 1000000.0);
}

UnifiedTraceItem::UnifiedTraceItem(int /*signalIndex*/, UnifiedTraceItem* parent)
    : m_parentItem(parent), m_isSignal(true), m_row(-1)
{
    if (m_parentItem) {
        m_timestamp = m_parentItem->timestamp();
    }
}

UnifiedTraceItem::~UnifiedTraceItem()
{
}

void UnifiedTraceItem::appendChild(std::shared_ptr<UnifiedTraceItem> child)
{
    child->setRow(m_childItems.size());
    m_childItems.append(child);
}

void UnifiedTraceItem::removeChildren(int row, int count)
{
    if (row >= 0 && (row + count) <= m_childItems.size()) {
        m_childItems.remove(row, count);
    }
}

std::shared_ptr<UnifiedTraceItem> UnifiedTraceItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int UnifiedTraceItem::childCount() const
{
    return m_childItems.size();
}

int UnifiedTraceItem::row() const
{
    return m_row;
}

UnifiedTraceItem* UnifiedTraceItem::parentItem()
{
    return m_parentItem;
}
