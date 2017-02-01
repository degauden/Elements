/**
 * @file ElementsLogging_test.cpp
 * @date January 14, 2014
 * @author Nikolaos Apostolakos
 */

#include <vector>                           // for std::vector
#include <string>                           // for std::string
#include <ostream>
#include <sstream>                          // for std::stringstream
#include <tuple>                            // for std::tuple
#include <ctime>
#include <fstream>
#include <iomanip>                          // for setprecision

#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>             // for boost::filesystem
#include <boost/version.hpp>                // for the BOOST_VERSION define

#include "ElementsKernel/Temporary.h"      // For TempDir
#include "ElementsKernel/Logging.h"
#include "ElementsKernel/MathConstants.h"  // For pi

using Elements::Logging;

using std::string;
using std::stringstream;
using std::vector;
using std::tuple;

using boost::filesystem::exists;

// A map to translate strings to logging levels
//std::map<string, Logging::Level> levelMap {
//  {"DEBUG", Logging::Level::DEBUG},
//  {"INFO", Logging::Level::INFO},
//  {"WARN", Logging::Level::WARN},
//  {"ERROR", Logging::Level::ERROR},
//  {"FATAL", Logging::Level::FATAL}
//};

// A class which takes over the given stream and keeps track of the log messages
// sent to it. It recovers the given stream in its previous state during destruction.
class LogMessageTracker {
 public:
  explicit LogMessageTracker(std::ostream& stream) :
      m_messages { }, m_stream(stream), m_old { stream.rdbuf(m_messages.rdbuf()) } {
  }
  ~LogMessageTracker() {
    m_stream.rdbuf(m_old);
  }
  void reset() {
    m_messages.str("");
    m_messages.clear();
  }
  vector<tuple<string, string, string, string>> getMessages() {

    using boost::algorithm::trim;
    using boost::algorithm::trim_left;

    vector<tuple<string, string, string, string>> messages;
    for (string line; std::getline(m_messages, line);) {
      string timestamp = line.substr(0, line.find(' '));
      line = line.substr(line.find(' ') + 1);
      string message = line.substr(line.find(':') + 1);
      trim_left(message);
      line = line.substr(0, line.find(':'));
      trim(line);
      string logLevelString = line.substr(line.rfind(' '));
      trim(logLevelString);
      // Logging::Level logLevel = levelMap[logLevelString];
      string name = line.substr(0, line.rfind(' '));
      trim(name);
      messages.push_back(
          std::make_tuple(timestamp, logLevelString, name, message));
    }
    return messages;
  }

 private:
  stringstream m_messages;
  std::ostream& m_stream;
  std::streambuf* m_old;
};


struct ElementsLogging_Fixture {
  Logging m_logger = Logging::getLogger("TestLogger");
  Elements::TempDir m_tmpdir;
  // This tracker will record all messages written in the stderr. The Elements
  // logging system guarantees that the messages will appear there.
  LogMessageTracker m_tracker { std::cerr };
  ElementsLogging_Fixture() {
    Logging::setLevel("INFO");
    Logging::setLogFile("");
  }
  ~ElementsLogging_Fixture() {
  }
};

class SetRandomSeed {
 public:
  SetRandomSeed() {
    std::srand(std::time(NULL));
  }
};

#if BOOST_VERSION >= 105900
BOOST_GLOBAL_FIXTURE(SetRandomSeed);
#else
BOOST_GLOBAL_FIXTURE(SetRandomSeed)
#endif


//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(ElementsLogging_test)

//-----------------------------------------------------------------------------
// Test that the names are set correctly
//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(loggerNames_test, ElementsLogging_Fixture) {


  // Given
  Logging logger2 = Logging::getLogger("TestLogger2");

  // When
  m_logger.info("From logger 1");
  logger2.info("From logger 2");

  // Then
  auto messages = m_tracker.getMessages();
  BOOST_CHECK_EQUAL(messages.size(), 2);
  string name1;
  std::tie(std::ignore, std::ignore, name1, std::ignore) = messages[0];
  BOOST_CHECK_EQUAL(name1, "TestLogger");
  string name2;
  std::tie(std::ignore, std::ignore, name2, std::ignore) = messages[1];
  BOOST_CHECK_EQUAL(name2, "TestLogger2");

}

//-----------------------------------------------------------------------------
// Test that the message text and level is correct
//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(messageTextAndLevel_test, ElementsLogging_Fixture) {

  // Given
  m_logger.setLevel("DEBUG");

  // When
  m_logger.debug("Debug message");
  m_logger.info("Info message");
  m_logger.warn("Warn message");
  m_logger.error("Error message");
  m_logger.fatal("Fatal message");
  m_logger.debug("Debug message with %d value", 15);
  m_logger.info("Info message with %d value", 15);
  m_logger.warn("Warn message with %d value", 15);
  m_logger.error("Error message with %d value", 15);
  m_logger.fatal("Fatal message with %d value", 15);
  m_logger.debug() << "Debug message with " << 15 << " value";
  m_logger.info() << "Info message with " << 15 << " value";
  m_logger.warn() << "Warn message with " << 15 << " value";
  m_logger.error() << "Error message with " << 15 << " value";
  m_logger.fatal() << "Fatal message with " << 15 << " value";
  m_logger.info() << "This is the logged Pi: " << 5  << " " << std::setprecision(12) << Elements::Units::pi;
  std::cout << "Pi:" << std::setprecision(9) << Elements::Units::pi << std::endl;;

  // Then
  auto messages = m_tracker.getMessages();
  BOOST_CHECK_EQUAL(messages.size(), 16);
  string logLevel;
  string message;
  std::tie(std::ignore, logLevel, std::ignore, message) = messages[0];
  BOOST_CHECK_EQUAL(logLevel, "DEBUG");
  BOOST_CHECK_EQUAL(message, "Debug message");
  std::tie(std::ignore, logLevel, std::ignore, message) = messages[1];
  BOOST_CHECK_EQUAL(logLevel, "INFO");
  BOOST_CHECK_EQUAL(message, "Info message");
  std::tie(std::ignore, logLevel, std::ignore, message) = messages[2];
  BOOST_CHECK_EQUAL(logLevel, "WARN");
  BOOST_CHECK_EQUAL(message, "Warn message");
  std::tie(std::ignore, logLevel, std::ignore, message) = messages[3];
  BOOST_CHECK_EQUAL(logLevel, "ERROR");
  BOOST_CHECK_EQUAL(message, "Error message");
  std::tie(std::ignore, logLevel, std::ignore, message) = messages[4];
  BOOST_CHECK_EQUAL(logLevel, "FATAL");
  BOOST_CHECK_EQUAL(message, "Fatal message");
  std::tie(std::ignore, logLevel, std::ignore, message) = messages[5];
  BOOST_CHECK_EQUAL(logLevel, "DEBUG");
  BOOST_CHECK_EQUAL(message, "Debug message with 15 value");
  std::tie(std::ignore, logLevel, std::ignore, message) = messages[6];
  BOOST_CHECK_EQUAL(logLevel, "INFO");
  BOOST_CHECK_EQUAL(message, "Info message with 15 value");
  std::tie(std::ignore, logLevel, std::ignore, message) = messages[7];
  BOOST_CHECK_EQUAL(logLevel, "WARN");
  BOOST_CHECK_EQUAL(message, "Warn message with 15 value");
  std::tie(std::ignore, logLevel, std::ignore, message) = messages[8];
  BOOST_CHECK_EQUAL(logLevel, "ERROR");
  BOOST_CHECK_EQUAL(message, "Error message with 15 value");
  std::tie(std::ignore, logLevel, std::ignore, message) = messages[9];
  BOOST_CHECK_EQUAL(logLevel, "FATAL");
  BOOST_CHECK_EQUAL(message, "Fatal message with 15 value");
  std::tie(std::ignore, logLevel, std::ignore, message) = messages[10];
  BOOST_CHECK_EQUAL(logLevel, "DEBUG");
  BOOST_CHECK_EQUAL(message, "Debug message with 15 value");
  std::tie(std::ignore, logLevel, std::ignore, message) = messages[11];
  BOOST_CHECK_EQUAL(logLevel, "INFO");
  BOOST_CHECK_EQUAL(message, "Info message with 15 value");
  std::tie(std::ignore, logLevel, std::ignore, message) = messages[12];
  BOOST_CHECK_EQUAL(logLevel, "WARN");
  BOOST_CHECK_EQUAL(message, "Warn message with 15 value");
  std::tie(std::ignore, logLevel, std::ignore, message) = messages[13];
  BOOST_CHECK_EQUAL(logLevel, "ERROR");
  BOOST_CHECK_EQUAL(message, "Error message with 15 value");
  std::tie(std::ignore, logLevel, std::ignore, message) = messages[14];
  BOOST_CHECK_EQUAL(logLevel, "FATAL");
  BOOST_CHECK_EQUAL(message, "Fatal message with 15 value");
  std::tie(std::ignore, logLevel, std::ignore, message) = messages[15];
  BOOST_CHECK_EQUAL(logLevel, "INFO");
  BOOST_CHECK_EQUAL(message, "This is the logged Pi: 5 3.14159265359");


}

//-----------------------------------------------------------------------------
// Test the setLevel method
//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(setLevel_test, ElementsLogging_Fixture) {

  // Given
  m_logger.setLevel("DEBUG");

  // When
  m_logger.debug("Debug message");
  m_logger.info("Info message");
  m_logger.warn("Warn message");
  m_logger.error("Error message");
  m_logger.fatal("Fatal message");

  // Then
  auto messages = m_tracker.getMessages();
  BOOST_CHECK_EQUAL(messages.size(), 5);

  // Given
  m_tracker.reset();
  m_logger.setLevel("INFO");

  // When
  m_logger.debug("Debug message");
  m_logger.info("Info message");
  m_logger.warn("Warn message");
  m_logger.error("Error message");
  m_logger.fatal("Fatal message");

  // Then
  messages = m_tracker.getMessages();
  BOOST_CHECK_EQUAL(messages.size(), 4);

  // Given
  m_tracker.reset();
  m_logger.setLevel("WARN");

  // When
  m_logger.debug("Debug message");
  m_logger.info("Info message");
  m_logger.warn("Warn message");
  m_logger.error("Error message");
  m_logger.fatal("Fatal message");

  // Then
  messages = m_tracker.getMessages();
  BOOST_CHECK_EQUAL(messages.size(), 3);

  // Given
  m_tracker.reset();
  m_logger.setLevel("ERROR");

  // When
  m_logger.debug("Debug message");
  m_logger.info("Info message");
  m_logger.warn("Warn message");
  m_logger.error("Error message");
  m_logger.fatal("Fatal message");

  // Then
  messages = m_tracker.getMessages();
  BOOST_CHECK_EQUAL(messages.size(), 2);

  // Given
  m_tracker.reset();
  m_logger.setLevel("FATAL");

  // When
  m_logger.debug("Debug message");
  m_logger.info("Info message");
  m_logger.warn("Warn message");
  m_logger.error("Error message");
  m_logger.fatal("Fatal message");

  // Then
  messages = m_tracker.getMessages();
  BOOST_CHECK_EQUAL(messages.size(), 1);
}

//-----------------------------------------------------------------------------
// Test logging in a file works correctly
//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(setLogFile_test, ElementsLogging_Fixture) {

  using boost::algorithm::ends_with;

  // Given
  stringstream logFileName { };
  logFileName << m_tmpdir.path().string() + "/"
              << std::time(NULL) << std::rand() << ".log";
  Logging::setLogFile(logFileName.str());

  // When
  m_logger.error("First message");
  m_logger.info("Second message");

  // Then
  BOOST_CHECK(exists(logFileName.str()));
  std::ifstream logFile { logFileName.str() };
  vector<string> lines { };
  string line;
  while (std::getline(logFile, line)) {
    lines.push_back(line);
  }
  logFile.close();
  BOOST_CHECK_EQUAL(lines.size(), 2);
  BOOST_CHECK(ends_with(lines[0], "First message"));
  BOOST_CHECK(ends_with(lines[1], "Second message"));
}

//-----------------------------------------------------------------------------
// Test logging is happening in a single file at a time
//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(singleLogFile_test, ElementsLogging_Fixture) {

  using boost::algorithm::ends_with;

  // Given
  stringstream logFileName1 { };
  logFileName1 << m_tmpdir.path().string() + "/"
               << std::time(NULL) << std::rand() << ".log";
  stringstream logFileName2 { };
  logFileName2 << m_tmpdir.path().string() + "/"
               << std::time(NULL) << std::rand() << ".log";

  // When

  Logging::setLogFile(logFileName1.str());
  m_logger.error("First message");
  Logging::setLogFile(logFileName2.str());
  m_logger.info("Second message");
  m_logger.info("Third message");

  // Then
  BOOST_CHECK(exists(logFileName1.str()));
  std::ifstream logFile1 { logFileName1.str() };
  vector<string> lines { };
  string line;
  while (std::getline(logFile1, line)) {
    lines.push_back(line);
  }
  logFile1.close();
  BOOST_CHECK_EQUAL(lines.size(), 1);
  BOOST_CHECK(ends_with(lines[0], "First message"));
  BOOST_CHECK(exists(logFileName2.str()));
  std::ifstream logFile2 { logFileName2.str() };
  lines = vector<string> { };
  while (std::getline(logFile2, line)) {
    lines.push_back(line);
  }
  logFile2.close();
  BOOST_CHECK_EQUAL(lines.size(), 2);
  BOOST_CHECK(ends_with(lines[0], "Second message"));
  BOOST_CHECK(ends_with(lines[1], "Third message"));
}

BOOST_AUTO_TEST_SUITE_END()
