#include <iostream>
#include "cxxopts.hpp"

cxxopts::ParseResult parse(int argc, char** argv)
{
    try 
    {
        cxxopts::Options options(argv[0], "this is a help string");
        options.positional_help("[positional help]").show_positional_help();

        options.allow_unrecognised_options()
            .add_options()
            ("f,from", "source directory", cxxopts::value<std::string>()->default_value("."))
            ("t,to", "destination")
            ("c,configuration", "project configuration", cxxopts::value<std::string>()->default_value("Release"))
            ("p,show-progress", "show copy progress ui", cxxopts::value<bool>()->default_value("false"));
    
        return options.parse(argc, argv);
    }
    catch (const cxxopts::OptionException& e) 
    {
        std::cout << "error: " << e.what() << std::endl;
        exit(1);
    }
}

int main(int argc, char** argv)
{
    auto result = parse(argc, argv);
    auto arguments = result.arguments();



    return 0;
}