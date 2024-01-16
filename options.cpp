#include "options.h"

#include "myapputils.h"

namespace {

[[noreturn]] void
die_with_help(std::string_view argv0, std::string_view msg) {
    ::die_with_help(argv0, msg, [argv0]() { help(stderr, argv0); });
}

struct ParsingContext {
    int argc{};
    const char * const * argv{};
    int curPos{};
};

double
parseDouble(const char * s, bool & isOk) {
    char * endptr = nullptr;
    errno = 0;
    double val = std::strtod(s, &endptr);
    if (errno) {
        perror("strtod");
    }
    isOk = !errno && *endptr == '\0';
    return val;
}

int
parseGain(const ParsingContext & ctx, Options & opts, Options::EditMode editMode) {
    const int numOfArgs = 1;

    if (ctx.curPos + numOfArgs >= ctx.argc) {
        die_with_help(opts.progname, "Not enough arguments for \"--add-gain\" or \"--set-gain\".");
    }

    if (opts.editMode != Options::EditMode::None) {
        die(opts.progname, "Only one option that modifies the gain be used.");
    }
    opts.editMode = editMode;

    bool isOk = false;
    const char * arg = ctx.argv[ctx.curPos + 1];
    double val = parseDouble(arg, isOk);
    if (!isOk) {
        printf("arg=%s, val=%f, isOk=%d\n", arg, val, isOk);
        die_with_help(opts.progname, "Invalid arguments for \"--add-gain\" or \"--set-gain\".");
    }

    opts.gain = val;
    return numOfArgs;
}

} // namespace

void
help(FILE * fp, std::string_view argv0) {
    fprintf(
            fp,
"Usage:\n"
"  %s INPUT_FILE\n"
"\n"
"  Mandatory:\n"
"    INPUT_FILE             Opus file in Ogg container.\n"
"\n"
"  Optional:\n"
"    -a | --add-gain GAIN   Add GAIN to the value already stored in the file.\n"
"    -s | --set-gain GAIN   Set GAIN, replacing the value stored in the file.\n"
"\n"
"    -h | --help            This help message\n"
"\n"
"Without optional arguments it prints current gain of the INPUT_FILE.\n"
"\n"
"The allowed values for gain stored in the file are floating point in range [-128, 127).\n",
            progname(argv0).c_str()
    );
}

Options
parseOptions(int argc, const char * const * argv) {
    ParsingContext ctx{ argc, argv, 1 };
    Options opts{};

    opts.progname = progname(argv[0]);

    for (; ctx.curPos < ctx.argc; ctx.curPos++) {
        std::string arg = ctx.argv[ctx.curPos];
        if (arg == "-a" || arg == "--add-gain") {
            int numOfArgs = parseGain(ctx, opts, Options::EditMode::Add);
            ctx.curPos += numOfArgs;
        }
        else if (arg == "-s" || arg == "--set-gain") {
            int numOfArgs = parseGain(ctx, opts, Options::EditMode::Set);
            ctx.curPos += numOfArgs;
        }
        else if (arg == "-h" || arg == "--help") {
            opts.doHelp = true;
        }
        else {
            if (!opts.input.empty()) {
                die_with_help(opts.progname, "Only one input file can be used.");
            }
            opts.input = arg;
        }
    }

    if (!opts.doHelp && opts.input.empty()) {
        die_with_help(opts.progname, "Input file is missing.");
    }
    return opts;
}
