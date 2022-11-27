
配置
https://github.com/kabinspace/AstroVim


教程
    https://mattermost.com/blog/how-to-install-and-set-up-neovim-for-code-editing/
    https://mattermost.com/blog/turning-neovim-into-a-full-fledged-code-editor-with-lua/

==============
--[[ init.lua ]]

-- LEADER
-- These keybindings need to be defined before the first /
-- is called; otherwise, it will default to "\"
vim.g.mapleader = ","
vim.g.localleader = "\\"

-- IMPORTS
-- require('vars')      -- Variables
-- require('opts')      -- Options
-- require('keys')      -- Keymaps
-- require('plug')      -- Plugins

==============
    vim.api.nvim_set_var to set internal variables.
    vim.api.nvim_set_option to set options.
    vim.api.nvim_set_keymap to set key mappings.
==============
    vim.g: maps to vim.api.nvim_set_var; sets global variables.
    vim.o: maps to vim.api.nvim_win_set_var; sets variables scoped to a given window.
    vim.go: maps to vim.api.nvim_set_option; equivalent to :setglobal
    vim.bo; maps to vim.api.nvim_buf_set_option; equivalent to :setlocal for buffer options.
    vim.wo: maps to vim.api.nvim_win_set_option; equivalent to :setlocal for window options
    vim.b; maps to vim.api.nvim_buf_set_var; sets variables scoped to a given buffer.
==============
查看变量信息
:help name-of-variable
==============
--[[ vars.lua ]]

local g = vim.g
g.t_co = 256
g.background = "dark"

==============
--[[ opts.lua ]]
local opt = vim.opt

-- [[ Context ]]
opt.colorcolumn = '80'           -- str:  Show col for max line length
opt.number = true                -- bool: Show line numbers
opt.relativenumber = true        -- bool: Show relative line numbers
opt.scrolloff = 4                -- int:  Min num lines of context
opt.signcolumn = "yes"           -- str:  Show the sign column

-- [[ Filetypes ]]
opt.encoding = 'utf8'            -- str:  String encoding to use
opt.fileencoding = 'utf8'        -- str:  File encoding to use

-- [[ Theme ]]
opt.syntax = "ON"                -- str:  Allow syntax highlighting
opt.termguicolors = true         -- bool: If term supports ui color then enable

-- [[ Search ]]
opt.ignorecase = true            -- bool: Ignore case in search patterns
opt.smartcase = true             -- bool: Override ignorecase if search contains capitals
opt.incsearch = true             -- bool: Use incremental search
opt.hlsearch = false             -- bool: Highlight search matches

-- [[ Whitespace ]]
opt.expandtab = true             -- bool: Use spaces instead of tabs
opt.shiftwidth = 4               -- num:  Size of an indent
opt.softtabstop = 4              -- num:  Number of spaces tabs count for in insert mode
opt.tabstop = 4                  -- num:  Number of spaces tabs count for

-- [[ Splits ]]
opt.splitright = true            -- bool: Place new window to right of current one
opt.splitbelow = true            -- bool: Place new window below the current one

==============

--[[ keys.lua ]]
local map = vim.api.nvim_set_keymap

-- remap the key used to leave insert mode
map('i', 'jk', '', {})

This function takes in four parameters:

    mode: the mode you want the key bind to apply to (e.g., insert mode, normal mode, command mode, visual mode).
    sequence: the sequence of keys to press.
    command: the command you want the keypresses to execute.
    options: an optional Lua table of options to configure (e.g., silent or noremap).
==============
