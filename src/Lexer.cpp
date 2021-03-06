/* This file is part of the parsevcf library (GPL v2 or later), see LICENSE */
#include "Lexer.h"

#include <iostream>
#include "Common.h"

using namespace std;

namespace parsevcf {

static const string unknown = "unknown value";

void error_message(const lexer& input, const string& message) {
	string exception_msg = "";
	exception_msg += "Error parsing at line: ";
	exception_msg += input.line;
	exception_msg += "\n";
	exception_msg += "Position: ";
	exception_msg += input.pos;
	exception_msg += "\n";
	exception_msg += message;
	exception_msg += "\n";

	parsevcf::exception::raise(message);
}

void error_missing(const lexer& input, const string& expecting = unknown) {
	string message = "Error, was expecting ";
	message += expecting;
	message += ".";
	error_message(input, message);
}

bool next_line(lexer& input) {
	if (input.pos == input.current_line.length() - 1) {
		input.current_line = "";
		getline(input.stream, input.current_line);
		input.current_line += '\n';
		input.pos = 0;
		input.line++;
		return true;
	}
	return false;
}

bool eof(lexer& input) {
	return input.current_line == "\n" && input.stream.eof();
}

bool next_character(lexer& input, const char& character) {
	if (character == '\n') {
		return next_line(input);
	}
	if (input.current_line[input.pos] == character) {
		input.pos++;
		return true;
	}
	return false;
}

bool next_string(lexer& input, const string& str) {
	if (input.current_line.substr(input.pos, str.length()) == str) {
		input.pos += str.length();
		return true;
	}
	return false;
}

bool next_string_until_one_of(lexer& input, string& output,
		const string& match) {
	size_t found = input.current_line.find_first_of(match, input.pos);
	if (found != string::npos) {
		output = input.current_line.substr(input.pos, found - input.pos);
		input.pos = found;
		return true;
	}
	return false;
}

bool next_string_until_newline(lexer& input, string& output) {
	return next_string_until_char(input, output, '\n');
}

bool next_string_until_char(lexer& input, string& output, const char& match) {
	size_t found = input.current_line.find_first_of(match, input.pos);
	if (found != string::npos) {
		output = input.current_line.substr(input.pos, found - input.pos);
		input.pos = found;
		return true;
	}
	return false;
}

} /* namespace parsevcf */
