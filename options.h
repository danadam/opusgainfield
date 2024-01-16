#ifndef OPUS_GAIN_FIELD_OPTIONS_H
#define OPUS_GAIN_FIELD_OPTIONS_H

#include <string>

void help(FILE * fp, std::string_view argv0);

struct Options {
    enum class EditMode {
        None,
        Add,
        Set
    };

    std::string progname;
    std::string input;
    EditMode editMode{};
    double gain{};
    bool doHelp{};
};

Options parseOptions(int argc, const char * const *argv);

#endif
