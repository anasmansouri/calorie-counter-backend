#include "utils/Result.hpp"

namespace cc::utils {
int convert_error_code_into_HTTP_Responses(ErrorCode e) {
    if (e == ErrorCode::InvalidInput) {
        return 400;
    } else if (e == ErrorCode::Unauthorized) {
        return 401;
    } else if (e == ErrorCode::Forbidden) {
        return 403;
    } else if (e == ErrorCode::NotFound) {
        return 404;
    } else if (e == ErrorCode::Conflict) {
        return 409;
    } else {
        return 500;
    }
}
} // namespace cc::utils