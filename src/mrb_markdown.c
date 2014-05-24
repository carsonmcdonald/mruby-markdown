/*
 * Copyright (c) 2013 Carson McDonald
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
 * of the Software, and to permit persons to whom the Software is furnished to do 
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN 
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/string.h>
#include <mruby/hash.h>

#include <stdio.h>

#include "sundown/src/markdown.h"
#include "sundown/html/html.h"
#include "sundown/src/buffer.h"

#define HASH_TRUE(mrb, hash, value) (mrb_obj_equal(mrb, mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_lit(mrb, value))), mrb_true_value()))

static void
mrb_sd_markdown_free(mrb_state *mrb, void *markdown)
{
  if(markdown)
  {
    sd_markdown_free((struct sd_markdown *)markdown);
  }
}

static const struct mrb_data_type sd_markdown_type = { "sd_markdown", mrb_sd_markdown_free };

static mrb_value
mrb_markdown_init(mrb_state *mrb, mrb_value self)
{
  mrb_value config_hash = mrb_nil_value();
  int argc = mrb_get_args(mrb, "|H", &config_hash);
  if(argc > 0 && mrb_type(config_hash) != MRB_TT_HASH)
  {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Invalid argument");
  }

  unsigned int sd_exts = 0;
  if(argc > 0)
  {
    if(HASH_TRUE(mrb, config_hash, "no_intra_emphassis")) sd_exts |= MKDEXT_NO_INTRA_EMPHASIS;
    if(HASH_TRUE(mrb, config_hash, "tables")) sd_exts |= MKDEXT_TABLES;
    if(HASH_TRUE(mrb, config_hash, "fenced_code")) sd_exts |= MKDEXT_FENCED_CODE;
    if(HASH_TRUE(mrb, config_hash, "autolink")) sd_exts |= MKDEXT_AUTOLINK;
    if(HASH_TRUE(mrb, config_hash, "strikethrough")) sd_exts |= MKDEXT_STRIKETHROUGH;
    if(HASH_TRUE(mrb, config_hash, "space_headers")) sd_exts |= MKDEXT_SPACE_HEADERS;
    if(HASH_TRUE(mrb, config_hash, "superscript")) sd_exts |= MKDEXT_SUPERSCRIPT;
    if(HASH_TRUE(mrb, config_hash, "lax_spacing")) sd_exts |= MKDEXT_LAX_SPACING;
  }

  struct sd_callbacks callbacks;
  struct html_renderopt options;
  sdhtml_renderer(&callbacks, &options, 0);

  struct sd_markdown *markdown = sd_markdown_new(sd_exts, 16, &callbacks, &options);

  if(!markdown) 
  {
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not create markdown instance.");
  }

  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "markdown"), mrb_obj_value(Data_Wrap_Struct(mrb, mrb->object_class, &sd_markdown_type, (void*)markdown)));

  return self;
}

static mrb_value
mrb_c_render(mrb_state *mrb, mrb_value self)
{
  mrb_value value_context = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "markdown"));

  struct sd_markdown* markdown = mrb_get_datatype(mrb, value_context, &sd_markdown_type);
  if(!markdown) 
  {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Markdown instance variable not set.");
  }

  mrb_value to_render = mrb_nil_value();
  mrb_get_args(mrb, "o", &to_render);
  if (mrb_nil_p(to_render) || mrb_type(to_render) != MRB_TT_STRING) 
  {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Invalid argument");
  }

  struct buf *ob = bufnew(64);
  sd_markdown_render(ob, (const uint8_t*) RSTRING_PTR(to_render), RSTRING_LEN(to_render), markdown);

  mrb_value rendered_output = mrb_str_new(mrb, (char*)ob->data, ob->size);
  bufrelease(ob);

  return rendered_output;
}

void
mrb_mruby_markdown_gem_init(mrb_state* mrb) 
{
  struct RClass *module_markdown = mrb_define_module(mrb, "Markdown");
  struct RClass *class_markdown = mrb_define_class_under(mrb, module_markdown, "Markdown", mrb->object_class);

  mrb_define_method(mrb, class_markdown, "initialize", mrb_markdown_init, ARGS_NONE());
  mrb_define_method(mrb, class_markdown, "render", mrb_c_render, ARGS_NONE());
}

void
mrb_mruby_markdown_gem_final(mrb_state* mrb) 
{ 
}
