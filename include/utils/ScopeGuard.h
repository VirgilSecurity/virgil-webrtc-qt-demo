/**
 * Copyright (C) 2015-2018 Virgil Security Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     (1) Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *     (2) Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *
 *     (3) Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Lead Maintainer: Virgil Security Inc. <support@virgilsecurity.com>
 */

#ifndef VIRGIL_CRYPTO_SCOPE_GUARD_H
#define VIRGIL_CRYPTO_SCOPE_GUARD_H

#include <functional>
#include <deque>
#include <type_traits>

/**
 * Implementation idea was taken from:
 * https://stackoverflow.com/questions/10270328/the-simplest-and-neatest-c11-scopeguard
 */
class ScopeGuard {
public:
    template <typename FunctionType>
    ScopeGuard(FunctionType &&undoFunc) try : undoFunc_(std::forward<FunctionType>(undoFunc)) {
    } catch (...) {
        undoFunc();
        throw;
    }

    ScopeGuard(ScopeGuard &&other) : undoFunc_(std::move(other.undoFunc_)) {
        other.undoFunc_ = nullptr;
    }

    ~ScopeGuard() {
        if (undoFunc_)
            undoFunc_(); // must not throw
    }

    void
    dismiss() noexcept {
        undoFunc_ = nullptr;
    }

    ScopeGuard(const ScopeGuard &) = delete;

    void
    operator=(const ScopeGuard &) = delete;

private:
    std::function<void()> undoFunc_;
};

class ScopeGuardOnException {
public:
    template <typename FunctionType>
    ScopeGuardOnException(FunctionType &&undoFunc) try : undoFunc_(std::forward<FunctionType>(undoFunc)) {
    } catch (...) {
        undoFunc();
        throw;
    }

    ScopeGuardOnException(ScopeGuardOnException &&other) : undoFunc_(std::move(other.undoFunc_)) {
        other.undoFunc_ = nullptr;
    }

    ~ScopeGuardOnException() {
        if (std::uncaught_exception() && undoFunc_ != nullptr) {
            undoFunc_(); // must not throw
        }
    }

    void
    dismiss() noexcept {
        undoFunc_ = nullptr;
    }

    ScopeGuardOnException(const ScopeGuardOnException &) = delete;

    void
    operator=(const ScopeGuardOnException &) = delete;

private:
    std::function<void()> undoFunc_;
};

#endif // VIRGIL_CRYPTO_SCOPE_GUARD_H
