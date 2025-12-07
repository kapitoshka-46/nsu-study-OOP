#ifndef CMD_PARSER_H
#define CMD_PARSER_H

#include <filesystem>
#include <boost/program_options.hpp>

namespace cmd {
    namespace po = boost::program_options;
    namespace fs = std::filesystem;

    class CmdParser
    {
    public:
        CmdParser(int argc, char *argv[]);

        [[nodiscard]] bool PrintHelpIfRequired(std::ostream& os) const;
        [[nodiscard]] fs::path GetOutputFilename() const;
        [[nodiscard]] fs::path GetConfigurationFilename() const;
        [[nodiscard]] std::vector<fs::path> GetInputFileNames() const;

    private:
        po::options_description description_;
        po::variables_map variables_map_;
    };
}



#endif //CMD_PARSER_H
