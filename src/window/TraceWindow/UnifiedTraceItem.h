#pragma once

#include <QHash>
#include <QVector>
#include <memory>

#include "core/BusMessage.h"

// One row of the rolling trace log: either a received frame or, as a child row,
// one DBC signal of that frame.
class UnifiedTraceItem {
public:
    UnifiedTraceItem(const BusMessage& frame, UnifiedTraceItem* parent = nullptr);
    UnifiedTraceItem(int signalIndex, UnifiedTraceItem* parent); // signal child
    ~UnifiedTraceItem();

    void appendChild(std::shared_ptr<UnifiedTraceItem> child);
    void removeChildren(int row, int count);
    std::shared_ptr<UnifiedTraceItem> child(int row);
    int childCount() const;
    int row() const;
    void setRow(int row) { m_row = row; }
    UnifiedTraceItem* parentItem();

    bool isSignal() const { return m_isSignal; }
    const BusMessage& rawFrame() const { return m_rawFrame; }

    uint32_t globalIndex() const { return m_globalIndex; }
    void setGlobalIndex(uint32_t index) { m_globalIndex = index; }
    uint64_t timestamp() const { return m_timestamp; }
    void setTimestamp(uint64_t ts) { m_timestamp = ts; }
    uint64_t prevSameIdTimestamp() const { return m_prevSameIdTimestamp; }
    void setPrevSameIdTimestamp(uint64_t ts) { m_prevSameIdTimestamp = ts; }
    const BusMessage& prevSameIdFrame() const { return m_prevSameIdFrame; }
    void setPrevSameIdFrame(const BusMessage &msg) { m_prevSameIdFrame = msg; }

private:
    QVector<std::shared_ptr<UnifiedTraceItem>> m_childItems;
    UnifiedTraceItem* m_parentItem;

    bool m_isSignal = false;
    BusMessage m_rawFrame;

    uint32_t m_globalIndex = 0;
    uint64_t m_timestamp = 0;
    uint64_t m_prevSameIdTimestamp = 0;
    BusMessage m_prevSameIdFrame;
    int m_row = -1;
};
