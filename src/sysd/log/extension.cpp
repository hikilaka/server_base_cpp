#include "sysd/log/extension.hpp"

#include <chrono>
#include <iomanip>

#include <boost/core/null_deleter.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/log/common.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include "sysd/term/color.hpp"

void color_formatter(const boost::log::record_view &record,
                     boost::log::formatting_ostream &stream) {
    auto now = std::chrono::system_clock::now();
    auto timepoint = std::chrono::system_clock::to_time_t(now);

    stream << sysd::term::color::gray<> << "[";
    stream << std::put_time(std::localtime(&timepoint), "%m/%e/%Y - %H:%M:%S");
    stream << "] " << sysd::term::color::clear<>;

    auto severity = record[boost::log::trivial::severity];

    if (severity) {
        switch (severity.get()) {
        case boost::log::trivial::severity_level::debug:
            stream << sysd::term::color::cyan<>;
            break;
        case boost::log::trivial::severity_level::info:
            stream << sysd::term::color::green<>;
            break;
        case boost::log::trivial::severity_level::warning:
            stream << sysd::term::color::yellow<>;
            break;
        case boost::log::trivial::severity_level::trace:
        case boost::log::trivial::severity_level::error:
        case boost::log::trivial::severity_level::fatal:
            stream << sysd::term::color::red<>;
            break;
        }

        stream << "[" << severity << "] " << sysd::term::color::clear<>;
    }

    stream << sysd::term::color::white<> <<
        record[boost::log::expressions::smessage] <<
        sysd::term::color::clear<>;
}

void sysd::log::add_extensions() {
    using namespace boost::log;
    using text_sink = sinks::synchronous_sink<sinks::text_ostream_backend>;

    auto sink = boost::make_shared<text_sink>();
    sink->locked_backend()
        ->add_stream(boost::shared_ptr<std::ostream>(&std::clog,
                                                     boost::null_deleter()));
    sink->set_formatter(&color_formatter);
    boost::log::core::get()->add_sink(sink);
}

