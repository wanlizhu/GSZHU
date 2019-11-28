#pragma once

//! Maximum number of color attachments allowed for render targets.
#define WANLIX_MAX_NUM_COLOR_ATTACHMENTS (8u)

//! Maximum number of attachments allowed for render targets (color attachments and depth-stencil attachment).
#define WANLIX_MAX_NUM_ATTACHMENTS ((WANLIX_MAX_NUM_COLOR_ATTACHMENTS) + 1u)

//! Maximum number of viewports and scissors.
#define WANLIX_MAX_NUM_VIEWPORTS_AND_SCISSORS (16u)

//! Maximum number of samples for multi-sampled rendering.
#define WANLIX_MAX_NUM_SAMPLES (64u)

//! Maximum number of stream-output buffers.
#define WANLIX_MAX_NUM_SO_BUFFERS (4u)