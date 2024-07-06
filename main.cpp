#include <iostream>
#include <string>
#include <fstream>
#include "include/stack.hpp"
#include "include/node.hpp"

std::string getName(std::string *filename)
{
    std::string name = "";
    for (int i = 0; (*filename)[i] != '.'; i++)
        name += (*filename)[i];
    return name;
}

void checkFile(std::string *filename)
{
    
    std::ifstream file;
    file.open(*filename);

    std::string logfile = getName(filename) + ".log";
    std::ofstream log;
    log.open(logfile);

    // std::ofstream charlog;
    // charlog.open("test/character.log");

    char ch;
    bool in_tag = false, closing = false;
    int errors = 0, line = 1;
    // int counter = 0;

    std::string tag_name;
    std::string last_tag;
    eda::Stack stack;

    while (file.good())
    {
        file.get(ch);
        switch (ch)
        {
        case '<':
            in_tag = true;
            tag_name = "";
            break;

        case '>':
            if (closing)
            {
                last_tag = stack.top()->getData();

                // std::cout << "Last: ";
                // stack.top()->print();
                // std::cout << std::endl;

                if (last_tag != tag_name)
                {
                    log << "Error in line " << line;
                    log << ": expecting </" << last_tag;
                    log << ">, got </" << tag_name << ">\n";
                    errors++;
                }
                else
                {
                    log << "tag <" << tag_name << "> ok" << std::endl;

                    // std::cout << "Actual: ";
                    // stack.top()->print();
                    // std::cout << std::endl;

                    stack.pop();
                }
                closing = false;
            } else {
                // std::cout << "Line: " << line << " pushing: " << tag_name << std::endl;
                stack.push(tag_name);
            }
            in_tag = false;
            break;

        case '/':
            closing = true;
            break;

        case '\n':
            line++;
            // counter = 0;
            break;

        default:
            if (in_tag)
                tag_name += ch;
            break;
        // counter ++;
        }
        // charlog << " char: " << ch;
        // charlog << " - line: " << line;
        // charlog << " - pos: " << counter;
        // charlog << " - in_tag: " << in_tag;
        // charlog << " - closing: " << closing << std::endl;
    }

    while (!stack.isEmpty())
    {
        tag_name = stack.top()->getData();
        log << "Error: tag <" << tag_name << "> never closed" << std::endl;
        stack.pop();
        errors++;
    }

    if (!errors)
    {
        log << "0 errors" << std::endl;
    }
    std::cout << "output -> " << logfile << std::endl;
}

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        std::string filename = argv[argc - 1];
        checkFile(&filename);
        return 0;
    }
    std::cout << "No file provided. Checking test files in the test/ directory." << std::endl;
    for (int test = 1; test <= 3; test++) {
        std::string testfile = "test/test" + std::to_string(test) + ".html";
        checkFile(&testfile);
    }
    return 0;
}