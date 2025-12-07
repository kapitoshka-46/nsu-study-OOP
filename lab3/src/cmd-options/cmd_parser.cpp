#include "cmd_parser.h"
#include "../converters-factory/converter_factory.h"

using namespace cmd;

CmdParser::CmdParser(int argc, char *argv[]) :description_("General options"){
    description_.add_options()
    ("help, h", "print help message")
    ("config, c", po::value<fs::path>()->required(), "specify config file")
    ("output, o", po::value<fs::path>()->default_value("output.wav"));

    po::positional_options_description pos_desc;
    pos_desc.add("input_files", -1);

    po::store(po::command_line_parser {argc, argv}.
        options(description_).
        positional(pos_desc).
        run(),
        variables_map_);

    try {
        po::notify(variables_map_);
    }
    catch (const po::required_option &e) {

        if (not variables_map_.contains("help")) {
            throw;
        }
    }
}

bool CmdParser::PrintHelpIfRequired(std::ostream &os) const {
    if (not variables_map_.contains("help")) {
        return false;
    }
    os << description_ << "\n";
    converter::ConverterFactory::PrintHelp(os);
    os << "\n";
    return true;
}


