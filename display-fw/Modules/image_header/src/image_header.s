.syntax unified
.cpu cortex-m33
.fpu fpv5-sp-d16
.thumb

// This must be linked at the beginning of the user flash
.section .image_header,"aS"
image_header_skip_indicator:
	.asciz	"VPX"
image_header_image_size:
	.word   0
image_header_image_digest:
	.space  0x8 * 4
