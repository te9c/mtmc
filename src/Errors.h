#pragma once

#include <stdexcept>

struct ApiError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct AuthenticationError : public ApiError {
    using ApiError::ApiError;
};

struct InvalidCredentialsError : public AuthenticationError {
    using AuthenticationError::AuthenticationError;
};

struct BadCookieError : public AuthenticationError {
    using AuthenticationError::AuthenticationError;
};
