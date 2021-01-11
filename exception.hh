#ifndef _YSL_EXCEPTION_HH
#define _YSL_EXCEPTION_HH

#include <exception>

#include "string.hh"

namespace ysl {
    class Exception : public std::exception {
        struct PtrWrapper {
            const char *str;

            PtrWrapper() = delete;
            PtrWrapper(const char *ptr) : str(ptr) {}
            ~PtrWrapper() {}
        };

        union Data {
            const PtrWrapper ptr;
            const String str;

            ~Data() {}
        };

        const bool m_is_ptr;
        const Data m_data;
    public:
        Exception(const char *ptr)
            : m_is_ptr(true), m_data({ .ptr = PtrWrapper(ptr) }) {}
        Exception(String str)
            : m_is_ptr(false), m_data({ .str = str }) {}

        virtual const char *what() const throw() override {
            if (m_is_ptr) {
                return m_data.ptr.str;
            } else {
                return m_data.str.as_str();
            }
        }

        ~Exception() {
            // this kinda stinks but okay
            if (m_is_ptr) {
                m_data.ptr.~PtrWrapper();
            } else {
                m_data.str.~String();
            }
        }
    };
}

#endif
