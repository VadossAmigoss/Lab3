#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <iomanip>

using namespace std;

struct token {
    string lexeme;
    string type;
};

vector<token> numbers;
vector<token> string_constants;
vector<token> char_constants;
vector<token> preprocessor_directives;
vector<token> comments;
vector<token> reserved_words;
vector<token> operators;
vector<token> separators;
vector<token> identifiers;
vector<token> errors;

const vector<string> reserved_words_list = {
    "dim", "if", "then", "else", "end", "for", "next", "do", "loop", "while",
    "wend", "function", "sub", "return", "true", "false", "and", "or", "not", "new"
};

const vector<string> operators_list = {
    "+", "-", "*", "/", "=", "<>", "<", ">", "<=", ">=", "And", "Or", "Not"
};

const vector<string> separators_list = {
    "(", ")", ",", ";", ".", ":", "\n"
};

void classify_token(const string& token) {
    regex number_regex(R"(\d+(\.\d+)?([eE][-+]?\d+)?)");
    regex hex_number_regex(R"(&[Hh][0-9A-Fa-f]+)");
    regex string_regex(R"("[^"]*")");
    regex char_regex(R"('[^']*')");
    regex preprocessor_regex(R"(#\w+)");
    regex identifier_regex(R"([A-Za-z_]\w*)");
    regex comment_regex(R"(\'[^\n]*)");

    if (regex_match(token, number_regex) || regex_match(token, hex_number_regex)) {
        numbers.push_back({ token, "Number" });
    }
    else if (regex_match(token, string_regex)) {
        string_constants.push_back({ token, "String Constant" });
    }
    else if (regex_match(token, char_regex)) {
        char_constants.push_back({ token, "Char Constant" });
    }
    else if (regex_match(token, preprocessor_regex)) {
        preprocessor_directives.push_back({ token, "Preprocessor Directive" });
    }
    else if (regex_match(token, comment_regex)) {
        comments.push_back({ token, "Comment" });
    }
    else if (find(reserved_words_list.begin(), reserved_words_list.end(), token) != reserved_words_list.end()) {
        reserved_words.push_back({ token, "Reserved Word" });
    }
    else if (find(operators_list.begin(), operators_list.end(), token) != operators_list.end()) {
        operators.push_back({ token, "Operator" });
    }
    else if (find(separators_list.begin(), separators_list.end(), token) != separators_list.end()) {
        separators.push_back({ token, "Separator" });
    }
    else if (regex_match(token, identifier_regex)) {
        identifiers.push_back({ token, "Identifier" });
    }
    else {
        errors.push_back({ token, "Unrecognized Token" });
    }
}

vector<string> tokenize(const string& code) {
    vector<string> tokens;
    regex token_regex(R"(\s+|([A-Za-z_]\w*)|(\d+(\.\d+)?([eE][-+]?\d+)?)|&[Hh][0-9A-Fa-f]+|("[^"]*")|('[^']*')|(#\w+)|(\[.*?\])|(\'.*?(\n|$))|(\+\+|--|&&|\|\==|!=|<=|>=)|([+\-*/=<>:;,()\[\]{}]))");    sregex_iterator iter(code.begin(), code.end(), token_regex);
    sregex_iterator end;

    while (iter != end) {
        for (size_t i = 1; i < iter->size(); ++i) {
            if ((*iter)[i].matched) {
                tokens.push_back((*iter)[i].str());
            }
        }
        ++iter;
    }

    return tokens;
}

void process_code(const string& code) {
    const auto tokens = tokenize(code);

    for (const auto& token : tokens) {
        classify_token(token);
    }
}

void print_tokens(const vector<token>& tokens, const string& header) {
    cout << header << endl;
    for (const auto& token : tokens) {
        cout << setw(20) << left << token.lexeme << token.type << endl;
    }
    cout << endl;
}
void print_all_tokens() {
    print_tokens(numbers, "Numbers:");
    print_tokens(string_constants, "String Constants:");
    print_tokens(char_constants, "Char Constants:");
    print_tokens(preprocessor_directives, "Preprocessor Directives:");
    print_tokens(comments, "Comments:");
    print_tokens(reserved_words, "Reserved Words:");
    print_tokens(operators, "Operators:");
    print_tokens(separators, "Separators:");
    print_tokens(identifiers, "Identifiers:");
    print_tokens(errors, "Errors:");
}

int main()
{
    const string code = R"( 'x' As String
                        x = "Hello"
                        Dim y As Float
                        y = 10.6
                         y > 0 Then ) (
                            y = y + 1
                        end if 
                        #define MAX 100
                        ' comment)";

    process_code(code);
    print_all_tokens();

    return 0;
}