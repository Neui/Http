/**
 * @file RequestTests.cpp
 *
 * This module contains the unit tests of the
 * Http::Request structure.
 *
 * © 2018 by Richard Walters
 */

#include <gtest/gtest.h>
#include <Http/Request.hpp>
#include <SystemAbstractions/StringExtensions.hpp>

TEST(RequestTests, IsCompleteOrError) {
    Http::Request request;
    request.state = Http::Request::State::Complete;
    EXPECT_TRUE(request.IsCompleteOrError());
    request.state = Http::Request::State::Error;
    EXPECT_TRUE(request.IsCompleteOrError());
    request.state = Http::Request::State::Headers;
    EXPECT_FALSE(request.IsCompleteOrError());
    request.state = Http::Request::State::RequestLine;
    EXPECT_FALSE(request.IsCompleteOrError());
    request.state = Http::Request::State::Body;
    EXPECT_FALSE(request.IsCompleteOrError());
}

TEST(RequestTests, GenerateGetRequest) {
    Http::Request request;
    request.method = "GET";
    ASSERT_TRUE(request.target.ParseFromString("/foo"));
    request.headers.SetHeader("Host", "www.example.com");
    request.headers.SetHeader("Content-Type", "text/plain");
    ASSERT_EQ(
        "GET /foo HTTP/1.1\r\n"
        "Host: www.example.com\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n",
        request.Generate()
    );
}

TEST(RequestTests, GeneratePutRequest) {
    Http::Request request;
    request.method = "PUT";
    ASSERT_TRUE(request.target.ParseFromString("/foo"));
    request.headers.SetHeader("Host", "www.example.com");
    request.headers.SetHeader("Content-Type", "text/plain");
    request.body = "FeelsGoodMan";
    request.headers.AddHeader("Content-Length", SystemAbstractions::sprintf("%zu", request.body.size()));
    ASSERT_EQ(
        SystemAbstractions::sprintf(
            "PUT /foo HTTP/1.1\r\n"
            "Host: www.example.com\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: %zu\r\n"
            "\r\n"
            "FeelsGoodMan",
            request.body.size()
        ),
        request.Generate()
    );
}
