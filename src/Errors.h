#pragma once

#include <stdexcept>

struct ApiError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct InvalidCredentialsError : public ApiError {
    using ApiError::ApiError;
};
