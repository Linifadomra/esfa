#include <stdexcept>
#include <string>

namespace esfa {

struct Error : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct UnknownFormatError : Error {
    UnknownFormatError(std::string kind_, std::string format_)
        : Error("esfa::Registry: no " + kind_ + " registered for type '" + format_ + "'"),
          kind(std::move(kind_)),
          format(std::move(format_)) {}
    std::string kind;
    std::string format;
};

struct SwapFailureError : Error {
    SwapFailureError(std::string format_, std::string reason_)
        : Error("esfa::Registry: " + format_ + " failed to swap: '" + reason_ + "'"),
          format(std::move(format_)),
          reason(std::move(reason_)) {}
    std::string format;
    std::string reason;
};

struct DuplicateError : Error {
    DuplicateError(std::string kind_, std::string format_)
        : Error("There is already a " + kind_ + " registered for format: " + format_),
          kind(std::move(kind_)),
          format(std::move(format_)) {}
    std::string kind;
    std::string format;
};

enum class StreamErrorKind {
    Open,
    Read,
    Write,
    Seek,
    EndOfStream,
    Closed,
};

enum class StreamType {
    File,
    Memory,
    Bounded,
};

inline const char* ToString(StreamType t) {
    switch (t) {
        case StreamType::File:    return "file";
        case StreamType::Memory:  return "memory";
        case StreamType::Bounded: return "bounded";
    }
    return "unknown";
}

inline const char* ToString(StreamErrorKind k) {
    switch (k) {
        case StreamErrorKind::Open:        return "open";
        case StreamErrorKind::Read:        return "read";
        case StreamErrorKind::Write:       return "write";
        case StreamErrorKind::Seek:        return "seek";
        case StreamErrorKind::EndOfStream: return "end of stream";
        case StreamErrorKind::Closed:      return "closed";
    }
    return "unknown";
}

struct StreamError : Error {
    StreamError(StreamType type_, StreamErrorKind kind_, std::string message_)
        : Error(std::string("esfa::Stream(") + ToString(type_) + "): " +
                ToString(kind_) + " error: " + message_),
          type(type_),
          kind(kind_),
          message(std::move(message_)) {}
    StreamType type;
    StreamErrorKind kind;
    std::string message;
};

} // namespace esfa
