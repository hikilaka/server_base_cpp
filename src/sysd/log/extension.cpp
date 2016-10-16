#include "sysd/log/extension.hpp"

#include <boost/core/null_deleter.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include "sysd/term/color.hpp"

void color_formatter(const boost::log::record_view &record,
                     boost::log::formatting_ostream &stream) {
    stream << sysd::term::color::gray<> << "[";
    stream << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp",
                                                                                  "%m:%d:%Y - %H:%M:%S");
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

