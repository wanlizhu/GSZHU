#pragma once

// Comment/Uncomment to enable certain feature
// These features do not necessarily improve performance, they are for testing purpose
// Please read each feature's comment carefully

#define SUBMIT_COMMAND_PER_DRAW_VK
//#define ENABLE_TRANSFER_QUEUE_VK // requires SUBMIT_COMMAND_PER_DRAW_VK
//#define POOLED_TEXTURE_VK		 // requires ENABLE_TRANSFER_QUEUE_VK

// Larger number means less frequent submission, 1 is "equivalent" to undefined, too large would degenerate to one submit per frame
//#define CONTROLLED_SUBMIT_RATE_VK 4			 // requires SUBMIT_COMMAND_PER_DRAW_VK, incompatible with ENABLE_TRANSFER_QUEUE_VK when POOLED_TEXTURE_VK is not enabled;
//#define CONTROLLED_TRANSFER_SUBMIT_RATE_VK 2 // requires ENABLE_TRANSFER_QUEUE_VK and CONTROLLED_SUBMIT_RATE_VK; if this is enabled, CONTROLLED_SUBMIT_RATE_VK must be a multiple of CONTROLLED_TRANSFER_SUBMIT_RATE_VK (x1 included)
//#define COMPACT_COMMAND_BUFFER_VK			 // requires CONTROLLED_SUBMIT_RATE_VK, compatible with CONTROLLED_TRANSFER_SUBMIT_RATE_VK

// Compatible with all other features
#define DIRECT_MAPPED_BUFFER_VK	
#define POOLED_STAGING_BUFFER_VK