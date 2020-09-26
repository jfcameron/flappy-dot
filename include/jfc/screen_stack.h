// © 2020 Joseph Cameron - All Rights Reserved

#ifndef SCREEN_STACK_H
#define SCREEN_STACK_H

#include <stack>
#include <memory>

#include <jfc/screen.h>

using screen_ptr_type = std::shared_ptr<gdk::screen>;
using screen_stack_type = std::stack<screen_ptr_type>;
using screen_stack_ptr_type = std::shared_ptr<screen_stack_type>;

#endif
