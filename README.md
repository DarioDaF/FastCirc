# Fast circular buffer

This library is a simple circular buffer implementation that maps adjacent virtual memory to the same buffer so that any single operation can be without wrap

## OS Support

This library support for now is only Linux with ANONYMOUS mapping

## Credit

This implementation is based on https://lo.calho.st/quick-hacks/employing-black-magic-in-the-linux-page-table/ by Travis Mick
