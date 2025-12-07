#ifndef CMD_PARSER_H
#define CMD_PARSER_H

#include <filesystem>
#include <boost/program_options.hpp>

namespace opt {
    namespace po = boost::program_options;
    namespace fs = std::filesystem;

    class Options
    {
    public:
        Options(int argc, char *argv[]);

        [[nodiscard]] bool PrintHelpIfRequired(std::ostream& os) const;
        [[nodiscard]] const fs::path &GetOutputFilename() const;
        [[nodiscard]] const fs::path &GetConfigurationFilename() const;
        [[nodiscard]] const std::vector<fs::path> &GetInputFileNames() const;

    private:
        po::options_description description_;
        po::variables_map variables_map_;
    };
}



#endif //CMD_PARSER_H
