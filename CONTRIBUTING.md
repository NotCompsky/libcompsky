# How to Contribute

## Testing

In particular, platforms I do not have access to but still aim to support (such as Windows, MacOS, and BSD).

## Submitting Bug Reports

## Submitting Feature Requests

## Submitting Pull Requests

### Encouraged

### Discouraged

### Coding Conventions

#### Tabs vs Spaces

Spaces must be used for right indents. For left indents, each indent level must be either one single tab or four spaces.

#### Spacing between parameters

Not too important, but if you can, if adding spacing, or qualifiers such as parenthesis makes the code more readable, do it.

For instance, adding two spaces between parameters if some parameters contain spaces themselves.

Examples:

  * `{0, 1, 2}` rather than `{0,1,2}`
  * `3600 * 24 * 7` rather than `604800` if it represents the number of seconds in a week
  * `memcpy(buf + offset,  str,  strlen(str))` rather than `memcpy(buf + offset, str, strlen(str))`

#### C-Style vs C++-Style

Parts that use C-style code are deliberate; please do not replace them with 'C++-style' code without a good reason. For instance, do not 'upgrade' `char*` to `std::string` without a good reason.

#### Misc

Some parts of the code look less tidy but are used for compatibility reasons; such as writing nested `namespace`s a single layer at a time.
