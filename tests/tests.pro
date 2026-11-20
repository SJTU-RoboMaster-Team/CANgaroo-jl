TEMPLATE = subdirs

# Unit tests. Build and run everything with:
#   cd tests && qmake6 && make -j$(nproc) && make check
SUBDIRS += \
    bus_message_signal \
    bus_message_frame \
    can_db_signal \
    dbc_parser \
    can_db \
    trace_file_format \
    trace_line_format \
    trace_file_writer \
    trace_file_reader \
    trace_recorder \
    trigger_capture \
    decoders \
    ldf_parser \
    candle_driver \
    measurement_setup \
    update_checker
