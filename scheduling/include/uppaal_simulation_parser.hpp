#ifndef UPPAAL_SIMULATION_PARSER_H
#define UPPAAL_SIMULATION_PARSER_H

#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

namespace scheduling {

struct FormulaNotSatisfiedException : std::exception {
    const char *what() const noexcept override { return "Formula not satisfied"; }
};

class SimulationParseException : public std::exception {
    std::string message;

  public:
    SimulationParseException(const std::string &inmessage) : message(inmessage) {}

    const char *what() const noexcept override { return message.c_str(); }
};

struct NameNotFoundException : std::exception {
    const char *what() const noexcept override { return "Cannot find name"; }
};

struct TimeValuePair {
    double time;
    int value;
};

struct SimulationTrace {
    int number;
    std::vector<TimeValuePair> values;
};

struct SimulationExpression {
    std::string name;
    std::vector<SimulationTrace> runs;
};

class UppaalSimulationParser {
  public:
    std::vector<SimulationExpression> parse(const std::string &result, int formula_number);
    std::queue<TimeValuePair> findFirstRunAsQueue(const std::vector<SimulationExpression> &values,
                                                  const std::string &name);

  private:
    SimulationExpression parseValue(std::istream &ss, std::string &line);
};

} // namespace scheduling

#endif // UPPAAL_SIMULATION_PARSER_H
