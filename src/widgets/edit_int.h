#include "widgets/edit_pattern.h"

#define EDIT_INT_SEP 0
#define EDIT_INT_SEP_NR 0
#define EDIT_INT_DEFVAL "000"

static uint32_t edit_int_part_get_max_len(uint32_t index, widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return edit->saved_text.size;
}

static bool_t edit_int_part_value_is_valid(uint32_t index, int32_t v, widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return v >= edit->min && v <= edit->max;
}

static bool_t edit_int_part_len_is_valid(uint32_t index, uint32_t len, widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return len == edit->saved_text.size;
}

static wchar_t* edit_int_fix_part(uint32_t index, int32_t v, wchar_t* start, wchar_t* end,
                                  widget_t* widget) {
  edit_t* edit = EDIT(widget);
  const wchar_t* fix_value = NULL;
  fix_value = edit->saved_text.str;

  if (fix_value != NULL) {
    wcscpy(start, fix_value);
    end = start + wcslen(fix_value);
    *end = 0;
  }
  return end;
}

static ret_t edit_int_inc_value(widget_t* widget) {
  return edit_add_value_with_sep(widget, 1, EDIT_INT_SEP);
}

static ret_t edit_int_dec_value(widget_t* widget) {
  return edit_add_value_with_sep(widget, -1, EDIT_INT_SEP);
}

static ret_t edit_int_pre_input(widget_t* widget, uint32_t key) {
  return edit_pre_input_with_sep(widget, key, EDIT_INT_SEP);
}

static ret_t edit_int_fix_ex(widget_t* widget, bool_t strict) {
  return edit_pattern_fix_ex(widget, strict, EDIT_INT_DEFVAL, EDIT_INT_SEP, EDIT_INT_SEP_NR,
                             edit_int_part_get_max_len, edit_int_fix_part);
}

static bool_t edit_int_is_valid_char(widget_t* widget, wchar_t c) {
  edit_pattern_fix_ex(widget, FALSE, EDIT_INT_DEFVAL, EDIT_INT_SEP, EDIT_INT_SEP_NR,
                      edit_int_part_get_max_len, edit_int_fix_part);

  return edit_pattern_is_valid_char(widget, c, EDIT_INT_SEP, edit_int_part_get_max_len);
}

static bool_t edit_int_is_valid(widget_t* widget) {
  return edit_pattern_is_valid(widget, EDIT_INT_SEP, EDIT_INT_SEP_NR, edit_int_part_len_is_valid,
                               edit_int_part_value_is_valid);
}

static ret_t edit_int_fix(widget_t* widget) {
  return edit_int_fix_ex(widget, TRUE);
}
