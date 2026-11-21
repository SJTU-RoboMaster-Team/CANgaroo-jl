#pragma once

#include "BaseTraceViewModel.h"
#include "UnifiedTraceItem.h"
#include <QHash>
#include <QTimer>
#include <memory>

// Rolling log of every frame in the trace, newest at the bottom. Each frame can
// be expanded into its DBC signals.
class UnifiedTraceViewModel : public BaseTraceViewModel
{
    Q_OBJECT

public:
    explicit UnifiedTraceViewModel(Backend &backend);
    ~UnifiedTraceViewModel() override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    bool hasChildren(const QModelIndex &parent) const override;

    BusMessage getMessage(const QModelIndex &index) const override;

    QVariant data(const QModelIndex &index, int role) const override;

private slots:
    void onTraceDirty();
    void processNewMessages();
    void beforeRemove(int count);
    void afterRemove(int count);
    void beforeClear();
    void afterClear();
    void onSetupChanged();

private:
    std::shared_ptr<UnifiedTraceItem> m_rootItem;

    int m_lastProcessedIndex = -1;
    uint32_t m_globalIndexCounter = 1;
    uint64_t m_firstTimestamp = 0;
    uint64_t m_previousRowTimestamp = 0;
    int m_maxRows = 10000;

    QTimer m_processTimer;

    QHash<uint64_t, uint64_t>   m_prevTimestampByKey;
    QHash<uint64_t, BusMessage> m_prevMessageByKey;
    static uint64_t makeDeltaKey(const BusMessage &msg);

    QVariant data_DisplayRole(const QModelIndex &index, int role) const override;
    QVariant data_TextColorRole(const QModelIndex &index, int role) const override;
    QString formatUnifiedTimestamp(uint64_t ts, uint64_t prevTs) const;
};
