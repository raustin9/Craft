#pragma once

namespace compiler {
namespace core {

namespace utils {

template <typename DstT, typename SrcT>
bool is_type(const SrcT* src) {
    return dynamic_cast<const DstT*>(src) != nullptr;
}

} // namespace utils
} // namespace core
} // namespace compiler
