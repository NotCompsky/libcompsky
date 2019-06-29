# Description

This is a pre-processor for extracting named groups from a regex, and parsing their metadata into vectors.

These named groups should follow the Python syntax, but with greater flexibility in naming (the only forbidden characters in the group names are '>' and '\0', and multiple groups can share the same name).

Groups can optionally be flagged with a boolean. If so, unnamed groups are automatically flagged True, and only named groups whose names start with '!' are flagged False.
