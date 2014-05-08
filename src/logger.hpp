#ifndef QUORIDOR_LOGGER_HPP_
#define QUORIDOR_LOGGER_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/exceptions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

BOOST_LOG_GLOBAL_LOGGER(qlogger, boost::log::sources::logger)

#endif  // QUORIDOR_LOGGER_HPP_
