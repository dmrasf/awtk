#include "widgets/edit_pattern.h"

#define EDIT_float_SEP '.'
#define EDIT_float_SEP_NR 1
#define EDIT_float_DEFVAL "0.0"

static uint32_t edit_float_part_get_max_len(uint32_t index, widget_t* widget) {
  edit_t* edit = EDIT(widget);
  const wchar_t* ps = NULL;
  const wchar_t* pe = NULL;
  wstr_t* text = &(edit->saved_text);

  ps = text->str;
  pe = wcs_chr(ps, EDIT_float_SEP);
  if (index == 0) {
    return pe - ps;
  } else {
    ps = pe + 1;
    pe = wcs_chr(ps, EDIT_float_SEP);
    if (pe == NULL) {
      pe = ps + wcslen(ps);
    }
    return pe - ps;
  }
}

static bool_t edit_float_part_value_is_valid(uint32_t index, int32_t v, widget_t* widget) {
  edit_t* edit = EDIT(widget);
  static int32_t int_value = 0;
  double value = 0;
  uint8_t float_num = edit_float_part_get_max_len(1, widget);
  int64_t min = 1, max = 1, val = 1;

  if (index == 0) {
    int_value = v;
    return TRUE;
  } else if (index == 1) {
    value = v;

    while (float_num--) {
      value /= 10.0;
      min *= 10;
      max *= 10;
      val *= 10;
    }
    value += int_value;

    min = round(min * edit->min);
    max = round(max * edit->max);
    val = val * value;

    return value >= min && value <= max;
  }
  return FALSE;
}

static bool_t edit_float_part_len_is_valid(uint32_t index, uint32_t len, widget_t* widget) {
  edit_t* edit = EDIT(widget);
  const wchar_t* ps = NULL;
  const wchar_t* pe = NULL;
  wstr_t* text = &(edit->saved_text);

  ps = text->str;
  pe = wcs_chr(ps, EDIT_float_SEP);
  if (index == 0) {
    return len == pe - ps;
  } else {
    ps = pe + 1;
    pe = wcs_chr(ps, EDIT_float_SEP);
    if (pe == NULL) {
      pe = ps + wcslen(ps);
    }
    return len == pe - ps;
  }
}

static wchar_t* edit_float_fix_part(uint32_t index, int32_t v, wchar_t* start, wchar_t* end,
                                    widget_t* widget) {
  edit_t* edit = EDIT(widget);
  const wchar_t* ps = NULL;
  const wchar_t* pe = NULL;
  wstr_t* text = &(edit->saved_text);
  uint32_t len = end - start;

  ps = text->str;
  pe = wcs_chr(ps, EDIT_float_SEP);
  if (index == 0) {
  } else if (index == 1) {
    ps = pe + 1;
    pe = wcs_chr(ps, EDIT_float_SEP);
    if (pe == NULL) {
      pe = ps + wcslen(ps);
    }
  }
  len = pe - ps;

  wcsncpy(start, ps, len);
  end = start + len;
  *end = 0;
  return end;
}

static ret_t edit_float_inc_value(widget_t* widget) {
  return edit_add_value_with_sep(widget, 1, EDIT_float_SEP);
}

static ret_t edit_float_dec_value(widget_t* widget) {
  return edit_add_value_with_sep(widget, -1, EDIT_float_SEP);
}

static ret_t edit_float_pre_input(widget_t* widget, uint32_t key) {
  return edit_pre_input_with_sep(widget, key, EDIT_float_SEP);
}

static ret_t edit_float_fix_ex(widget_t* widget, bool_t strict) {
  return edit_pattern_fix_ex(widget, strict, EDIT_float_DEFVAL, EDIT_float_SEP, EDIT_float_SEP_NR,
                             edit_float_part_get_max_len, edit_float_fix_part);
}

static bool_t edit_float_is_valid_char(widget_t* widget, wchar_t c) {
  edit_pattern_fix_ex(widget, FALSE, EDIT_float_DEFVAL, EDIT_float_SEP, EDIT_float_SEP_NR,
                      edit_float_part_get_max_len, edit_float_fix_part);

  return edit_pattern_is_valid_char(widget, c, EDIT_float_SEP, edit_float_part_get_max_len);
}

static bool_t edit_float_is_valid(widget_t* widget) {
  return edit_pattern_is_valid(widget, EDIT_float_SEP, EDIT_float_SEP_NR,
                               edit_float_part_len_is_valid, edit_float_part_value_is_valid);
}

static ret_t edit_float_fix(widget_t* widget) {
  return edit_float_fix_ex(widget, TRUE);
}
