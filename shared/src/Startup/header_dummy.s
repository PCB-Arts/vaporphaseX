.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

// This must be linked at the beginning of the user flash
.section .image_header,"aS"
image_header_skip_indicator:
	.asciz	"VPO"
image_header_image_size:
	.word   0
image_header_image_digest:
	.space  0x8 * 4
