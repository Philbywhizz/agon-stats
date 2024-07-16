# agon-stats

This is a simple include file that you can include in your agon light assembly
project. It is built using the [ez80asm assembler](https://github.com/envenomator/agon-ez80asm)

I created this file to help me learn ez80 assembly. I would add the .STATS macro in my code where I needed to know the status of the registers. It's a poor mans hardware monitor :)

You can use the .STATS macro as many times as you like (I save the state for
each call).

I'm sure there is some optimization that can be done, and there is probably a few bugs in there as well (I'm still learning ez80 assembly myself). It has been a great learning exercise (especially with learning how to ix register works with indexing the return stack).
