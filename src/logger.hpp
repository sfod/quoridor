#ifndef QUORIDOR_LOGGER_HPP_
#define QUORIDOR_LOGGER_HPP_

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions/formatters/named_scope.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/exceptions.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

namespace blattrs = boost::log::attributes;
namespace blexpr = boost::log::expressions;

BOOST_LOG_ATTRIBUTE_KEYWORD(scope, "Scope", boost::log::attributes::named_scope::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", boost::log::trivial::severity_level)

#define BOOST_LOG_DEBUG(lg) BOOST_LOG_SEV((lg), boost::log::trivial::debug)
#define BOOST_LOG_INFO(lg) BOOST_LOG_SEV((lg), boost::log::trivial::info)
#define BOOST_LOG_ERROR(lg) BOOST_LOG_SEV((lg), boost::log::trivial::error)
#define BOOST_LOG_FATAL(lg) BOOST_LOG_SEV((lg), boost::log::trivial::fatal)

#endif  // QUORIDOR_LOGGER_HPP_
