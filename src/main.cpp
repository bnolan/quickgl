#include <SDL.h>
#include <chrono>
#include <iostream>
#include <opencv2/objdetect.hpp> // Include the header file for object detection
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>

extern "C" {

#define TGL_FEATURE_16_BITS 1
#include "GL/gl.h"
#include "quickjs.h"
#include "zbuffer.h"
#define CHAD_API_IMPL
#define CHAD_MATH_IMPL
#include "3dMath.h"
}

cv::CascadeClassifier face_cascade;

int winSizeX = 480;
int winSizeY = 480;

// Define the JavaScript code that generates the counter text
std::string templateCode = R"XX(

    var timePassed = 0.0;

Object.assign(TinyGL, {

		// Boolean values
		//

		FALSE: 0, 
		TRUE: 1, 

		// Data types
		//

		BYTE: 0x1400, 
		UNSIGNED_BYTE: 0x1401, 
		SHORT: 0x1402, 
		UNSIGNED_SHORT: 0x1403, 
		INT: 0x1404, 
		UNSIGNED_INT: 0x1405, 
		FLOAT: 0x1406, 
		DOUBLE: 0x140A, 
//		2_BYTES: 0x1407, 
//		3_BYTES: 0x1408, 
//		4_BYTES: 0x1409, 

		// Primitives
		//

		POINTS: 0x0000, 
		LINES: 0x0001, 
		LINE_LOOP: 0x0002, 
		LINE_STRIP: 0x0003, 
		TRIANGLES: 0x0004, 
		TRIANGLE_STRIP: 0x0005, 
		TRIANGLE_FAN: 0x0006, 
		QUADS: 0x0007, 
		QUAD_STRIP: 0x0008, 
		POLYGON: 0x0009, 
		EDGE_FLAG: 0x0B43, 

		// Vertex Arrays
		//

		VERTEX_ARRAY: 0x8074, 
		NORMAL_ARRAY: 0x8075, 
		COLOR_ARRAY: 0x8076, 
		INDEX_ARRAY: 0x8077, 
		TEXTURE_COORD_ARRAY: 0x8078, 
		EDGE_FLAG_ARRAY: 0x8079, 
		VERTEX_ARRAY_SIZE: 0x807A, 
		VERTEX_ARRAY_TYPE: 0x807B, 
		VERTEX_ARRAY_STRIDE: 0x807C, 
		VERTEX_ARRAY_COUNT: 0x807D, 
		NORMAL_ARRAY_TYPE: 0x807E, 
		NORMAL_ARRAY_STRIDE: 0x807F, 
		NORMAL_ARRAY_COUNT: 0x8080, 
		COLOR_ARRAY_SIZE: 0x8081, 
		COLOR_ARRAY_TYPE:  0x8082,
		COLOR_ARRAY_STRIDE:  0x8083,
		COLOR_ARRAY_COUNT:  0x8084,
		INDEX_ARRAY_TYPE:  0x8085,
		INDEX_ARRAY_STRIDE:  0x8086,
		INDEX_ARRAY_COUNT:  0x8087,
		TEXTURE_COORD_ARRAY_SIZE:  0x8088,
		TEXTURE_COORD_ARRAY_TYPE:  0x8089,
		TEXTURE_COORD_ARRAY_STRIDE:  0x808A,
		TEXTURE_COORD_ARRAY_COUNT:  0x808B,
		EDGE_FLAG_ARRAY_STRIDE:  0x808C,
		EDGE_FLAG_ARRAY_COUNT:  0x808D,
		VERTEX_ARRAY_POINTER:  0x808E,
		NORMAL_ARRAY_POINTER:  0x808F,
		COLOR_ARRAY_POINTER:  0x8090,
		INDEX_ARRAY_POINTER:  0x8091,
		TEXTURE_COORD_ARRAY_POINTER:  0x8092,
		EDGE_FLAG_ARRAY_POINTER:  0x8093,
		V2F: 0x2A20,
		V3F:  0x2A21,
		C4UB_V2F:  0x2A22,
		C4UB_V3F:  0x2A23,
		C3F_V3F:  0x2A24,
		N3F_V3F:  0x2A25,
		C4F_N3F_V3F:  0x2A26,
		T2F_V3F:  0x2A27,
		T4F_V4F:  0x2A28,
		T2F_C4UB_V3F:  0x2A29,
		T2F_C3F_V3F:  0x2A2A,
		T2F_N3F_V3F:  0x2A2B,
		T2F_C4F_N3F_V3F:  0x2A2C,
		T4F_C4F_N3F_V4F:  0x2A2D,

		// Matrix Modes
		//

		MATRIX_MODE:  0x0BA0,
		MODELVIEW:  0x1700,
		PROJECTION:  0x1701,
		TEXTURE:  0x1702,

		// Points
		//

		POINT_SMOOTH:  0x0B10,
		POINT_SIZE:  0x0B11,
		POINT_SIZE_GRANULARITY :  0x0B13,
		POINT_SIZE_RANGE:  0x0B12,

		// Lines
		//

		LINE_SMOOTH:  0x0B20,
		LINE_STIPPLE:  0x0B24,
		LINE_STIPPLE_PATTERN:  0x0B25,
		LINE_STIPPLE_REPEAT:  0x0B26,
		LINE_WIDTH:  0x0B21,
		LINE_WIDTH_GRANULARITY:  0x0B23,
		LINE_WIDTH_RANGE:  0x0B22,

		// Polygons
		//

		POINT:  0x1B00,
		LINE:  0x1B01,
		FILL:  0x1B02,
		CCW:  0x0901,
		CW:  0x0900,
		FRONT:  0x0404,
		BACK:  0x0405,
		CULL_FACE:  0x0B44,
		CULL_FACE_MODE:  0x0B45,
		POLYGON_SMOOTH:  0x0B41,
		POLYGON_STIPPLE:  0x0B42,
		FRONT_FACE:  0x0B46,
		POLYGON_MODE:  0x0B40,
		POLYGON_OFFSET_FACTOR:  0x3038,
		POLYGON_OFFSET_UNITS:  0x2A00,
		POLYGON_OFFSET_POINT:  0x2A01,
		POLYGON_OFFSET_LINE:  0x2A02,
		POLYGON_OFFSET_FILL:  0x8037,

		// Display Lists
		//

		COMPILE:  0x1300,
		COMPILE_AND_EXECUTE:  0x1301,
		LIST_BASE:  0x0B32,
		LIST_INDEX:  0x0B33,
		LIST_MODE:  0x0B30,

		// Depth buffer
		//

		NEVER:  0x0200,
		LESS:  0x0201,
		GEQUAL:  0x0206,
		LEQUAL:  0x0203,
		GREATER:  0x0204,
		NOTEQUAL:  0x0205,
		EQUAL:  0x0202,
		ALWAYS:  0x0207,
		DEPTH_TEST:  0x0B71,
		DEPTH_BITS:  0x0D56,
		DEPTH_CLEAR_VALUE:  0x0B73,
		DEPTH_FUNC:  0x0B74,
		DEPTH_RANGE:  0x0B70,
		DEPTH_WRITEMASK:  0x0B72,
		DEPTH_COMPONENT:  0x1902,

		// Lighting
		//

		LIGHTING:  0x0B50,
		LIGHT0:  0x4000,
		LIGHT1:  0x4001,
		LIGHT2:  0x4002,
		LIGHT3:  0x4003,
		LIGHT4:  0x4004,
		LIGHT5:  0x4005,
		LIGHT6:  0x4006,
		LIGHT7:  0x4007,
		SPOT_EXPONENT:  0x1205,
		SPOT_CUTOFF:  0x1206,
		CONSTANT_ATTENUATION:  0x1207,
		LINEAR_ATTENUATION:  0x1208,
		QUADRATIC_ATTENUATION:  0x1209,
		AMBIENT:  0x1200,
		DIFFUSE:  0x1201,
		SPECULAR:  0x1202,
		SHININESS:  0x1601,
		EMISSION:  0x1600,
		POSITION:  0x1203,
		SPOT_DIRECTION:  0x1204,
		AMBIENT_AND_DIFFUSE:  0x1602,
		COLOR_INDEXES:  0x1603,
		LIGHT_MODEL_TWO_SIDE:  0x0B52,
		LIGHT_MODEL_LOCAL_VIEWER:  0x0B51,
		LIGHT_MODEL_AMBIENT:  0x0B53,
		FRONT_AND_BACK:  0x0408,
		SHADE_MODEL:  0x0B54,
		FLAT:  0x1D00,
		SMOOTH:  0x1D01,
		COLOR_MATERIAL:  0x0B57,
		COLOR_MATERIAL_FACE:  0x0B55,
		COLOR_MATERIAL_PARAMETER:  0x0B56,
		NORMALIZE:  0x0BA1,

		// User clipping planes
		//

		CLIP_PLANE0:  0x3000,
		CLIP_PLANE1:  0x3001,
		CLIP_PLANE2:  0x3002,
		CLIP_PLANE3:  0x3003,
		CLIP_PLANE4:  0x3004,
		CLIP_PLANE5:  0x3005,

		// Accumulation buffer
		//

		ACCUM_RED_BITS:  0x0D58,
		ACCUM_GREEN_BITS:  0x0D59,
		ACCUM_BLUE_BITS:  0x0D5A,
		ACCUM_ALPHA_BITS:  0x0D5B,
		ACCUM_CLEAR_VALUE:  0x0B80,
		ACCUM:  0x0100,
		ADD:  0x0104,
		LOAD:  0x0101,
		MULT:  0x0103,
		RETURN:  0x0102,

		// Alpha testing
		//

		ALPHA_TEST:  0x0BC0,
		ALPHA_TEST_REF:  0x0BC2,
		ALPHA_TEST_FUNC:  0x0BC1,

		// Blending
		//

		BLEND:  0x0BE2,
		BLEND_SRC:  0x0BE1,
		BLEND_DST:  0x0BE0,
		ZERO:  0,
		ONE:  1,
		SRC_COLOR:  0x0300,
		ONE_MINUS_SRC_COLOR:  0x0301,
		DST_COLOR:  0x0306,
		ONE_MINUS_DST_COLOR:  0x0307,
		SRC_ALPHA:  0x0302,
		ONE_MINUS_SRC_ALPHA:  0x0303,
		DST_ALPHA:  0x0304,
		ONE_MINUS_DST_ALPHA:  0x0305,
		SRC_ALPHA_SATURATE:  0x0308,
		CONSTANT_COLOR:  0x8001,
		ONE_MINUS_CONSTANT_COLOR:  0x8002,
		CONSTANT_ALPHA:  0x8003,
		ONE_MINUS_CONSTANT_ALPHA:  0x8004,

		// Render Mode
		//

		FEEDBACK:  0x1C01,
		RENDER:  0x1C00,
		SELECT:  0x1C02,

		// Feedback
		//

//		2D:  0x0600,
//		3D:  0x0601,
//		3D_COLOR:  0x0602,
//		3D_COLOR_TEXTURE:  0x0603,
//		4D_COLOR_TEXTURE:  0x0604,
		POINT_TOKEN:  0x0701,
		LINE_TOKEN:  0x0702,
		LINE_RESET_TOKEN:  0x0707,
		POLYGON_TOKEN:  0x0703,
		BITMAP_TOKEN:  0x0704,
		DRAW_PIXEL_TOKEN:  0x0705,
		COPY_PIXEL_TOKEN:  0x0706,
		PASS_THROUGH_TOKEN:  0x0700,

		// Fog
		//

		FOG:  0x0B60,
		FOG_MODE:  0x0B65,
		FOG_DENSITY:  0x0B62,
		FOG_COLOR:  0x0B66,
		FOG_INDEX:  0x0B61,
		FOG_START:  0x0B63,
		FOG_END:  0x0B64,
		LINEAR:  0x2601,
		EXP:  0x0800,
		EXP2:  0x0801,

		// Logic Ops
		//

		LOGIC_OP:  0x0BF1,
		LOGIC_OP_MODE:  0x0BF0,
		CLEAR:  0x1500,
		SET:  0x150F,
		COPY:  0x1503,
		COPY_INVERTED:  0x150C,
		NOOP:  0x1505,
		INVERT:  0x150A,
		AND:  0x1501,
		NAND:  0x150E,
		OR:  0x1507,
		NOR:  0x1508,
		XOR:  0x1506,
		EQUIV:  0x1509,
		AND_REVERSE:  0x1502,
		AND_INVERTED:  0x1504,
		OR_REVERSE:  0x150B,
		OR_INVERTED:  0x150D,

		// Stencil
		//

		STENCIL_TEST:  0x0B90,
		STENCIL_WRITEMASK:  0x0B98,
		STENCIL_BITS:  0x0D57,
		STENCIL_FUNC:  0x0B92,
		STENCIL_VALUE_MASK:  0x0B93,
		STENCIL_REF:  0x0B97,
		STENCIL_FAIL:  0x0B94,
		STENCIL_PASS_DEPTH_PASS:  0x0B96,
		STENCIL_PASS_DEPTH_FAIL:  0x0B95,
		STENCIL_CLEAR_VALUE:  0x0B91,
		STENCIL_INDEX:  0x1901,
		KEEP:  0x1E00,
		REPLACE:  0x1E01,
		INCR:  0x1E02,
		DECR:  0x1E03,

		// Buffers, Pixel Drawing/Reading
		//

		NONE:  0,
		LEFT:  0x0406,
		RIGHT:  0x0407,
		FRONT: 0x0404,
		BACK: 0x0405,
		FRONT_AND_BACK: 0x0408, 
		FRONT_LEFT:  0x0400,
		FRONT_RIGHT:  0x0401,
		BACK_LEFT:  0x0402,
		BACK_RIGHT:  0x0403,
		AUX0:  0x0409,
		AUX1:  0x040A,
		AUX2:  0x040B,
		AUX3:  0x040C,
		COLOR_INDEX:  0x1900,
		RED:  0x1903,
		GREEN:  0x1904,
		BLUE:  0x1905,
		ALPHA:  0x1906,
		LUMINANCE:  0x1909,
		LUMINANCE_ALPHA:  0x190A,
		ALPHA_BITS:  0x0D55,
		RED_BITS:  0x0D52,
		GREEN_BITS:  0x0D53,
		BLUE_BITS:  0x0D54,
		INDEX_BITS:  0x0D51,
		SUBPIXEL_BITS:  0x0D50,
		AUX_BUFFERS:  0x0C00,
		READ_BUFFER:  0x0C02,
		DRAW_BUFFER:  0x0C01,
		DOUBLEBUFFER:  0x0C32,
		STEREO:  0x0C33,
		BITMAP:  0x1A00,
		COLOR:  0x1800,
		DEPTH:  0x1801,
		STENCIL:  0x1802,
		DITHER:  0x0BD0,
		RGB:  0x1907,
		RGBA:  0x1908,

		// Implementation limits
		//

		MAX_LIST_NESTING:  0x0B31,
		MAX_ATTRIB_STACK_DEPTH:  0x0D35,
		MAX_MODELVIEW_STACK_DEPTH:  0x0D36,
		MAX_NAME_STACK_DEPTH:  0x0D37,
		MAX_PROJECTION_STACK_DEPTH:  0x0D38,
		MAX_TEXTURE_STACK_DEPTH:  0x0D39,
		MAX_EVAL_ORDER:  0x0D30,
		MAX_LIGHTS:  0x0D31,
		MAX_CLIP_PLANES:  0x0D32,
		MAX_TEXTURE_SIZE:  0x0D33,
		MAX_PIXEL_MAP_TABLE:  0x0D34,
		MAX_VIEWPORT_DIMS:  0x0D3A,
		MAX_CLIENT_ATTRIB_STACK_DEPTH: 0x0D3B,

		// Gets
		//

		ATTRIB_STACK_DEPTH:  0x0BB0,
		COLOR_CLEAR_VALUE:  0x0C22,
		COLOR_WRITEMASK:  0x0C23,
		CURRENT_INDEX:  0x0B01,
		CURRENT_COLOR:  0x0B00,
		CURRENT_NORMAL:  0x0B02,
		CURRENT_RASTER_COLOR:  0x0B04,
		CURRENT_RASTER_DISTANCE:  0x0B09,
		CURRENT_RASTER_INDEX:  0x0B05,
		CURRENT_RASTER_POSITION:  0x0B07,
		CURRENT_RASTER_TEXTURE_COORDS: 0x0B06,
		CURRENT_RASTER_POSITION_VALID: 0x0B08,
		CURRENT_TEXTURE_COORDS:  0x0B03,
		INDEX_CLEAR_VALUE:  0x0C20,
		INDEX_MODE:  0x0C30,
		INDEX_WRITEMASK:  0x0C21,
		MODELVIEW_MATRIX:  0x0BA6,
		MODELVIEW_STACK_DEPTH:  0x0BA3,
		NAME_STACK_DEPTH:  0x0D70,
		PROJECTION_MATRIX:  0x0BA7,
		PROJECTION_STACK_DEPTH:  0x0BA4,
		RENDER_MODE:  0x0C40,
		RGBA_MODE:  0x0C31,
		TEXTURE_MATRIX:  0x0BA8,
		TEXTURE_STACK_DEPTH:  0x0BA5,
		VIEWPORT:  0x0BA2,

		// Evaluators
		//

		AUTO_NORMAL:  0x0D80,
		MAP1_COLOR_4:  0x0D90,
		MAP1_GRID_DOMAIN:  0x0DD0,
		MAP1_GRID_SEGMENTS:  0x0DD1,
		MAP1_INDEX:  0x0D91,
		MAP1_NORMAL:  0x0D92,
		MAP1_TEXTURE_COORD_1:  0x0D93,
		MAP1_TEXTURE_COORD_2:  0x0D94,
		MAP1_TEXTURE_COORD_3:  0x0D95,
		MAP1_TEXTURE_COORD_4:  0x0D96,
		MAP1_VERTEX_3:  0x0D97,
		MAP1_VERTEX_4:  0x0D98,
		MAP2_COLOR_4:  0x0DB0,
		MAP2_GRID_DOMAIN:  0x0DD2,
		MAP2_GRID_SEGMENTS:  0x0DD3,
		MAP2_INDEX:  0x0DB1,
		MAP2_NORMAL:  0x0DB2,
		MAP2_TEXTURE_COORD_1:  0x0DB3,
		MAP2_TEXTURE_COORD_2:  0x0DB4,
		MAP2_TEXTURE_COORD_3:  0x0DB5,
		MAP2_TEXTURE_COORD_4:  0x0DB6,
		MAP2_VERTEX_3:  0x0DB7,
		MAP2_VERTEX_4:  0x0DB8,
		COEFF:  0x0A00,
		DOMAIN:  0x0A02,
		ORDER:  0x0A01,

		// Hints
		//

		FOG_HINT:  0x0C54,
		LINE_SMOOTH_HINT:  0x0C52,
		PERSPECTIVE_CORRECTION_HINT:  0x0C50,
		POINT_SMOOTH_HINT:  0x0C51,
		POLYGON_SMOOTH_HINT:  0x0C53,
		DONT_CARE:  0x1100,
		FASTEST:  0x1101,
		NICEST:  0x1102,

		// Scissor box
		//

		SCISSOR_TEST:  0x0C11,
		SCISSOR_BOX:  0x0C10,

		// Pixel Mode / Transfer
		//

		MAP_COLOR:  0x0D10,
		MAP_STENCIL:  0x0D11,
		INDEX_SHIFT:  0x0D12,
		INDEX_OFFSET:  0x0D13,
		RED_SCALE:  0x0D14,
		RED_BIAS:  0x0D15,
		GREEN_SCALE:  0x0D18,
		GREEN_BIAS:  0x0D19,
		BLUE_SCALE:  0x0D1A,
		BLUE_BIAS:  0x0D1B,
		ALPHA_SCALE:  0x0D1C,
		ALPHA_BIAS:  0x0D1D,
		DEPTH_SCALE:  0x0D1E,
		DEPTH_BIAS:  0x0D1F,
		PIXEL_MAP_S_TO_S_SIZE:  0x0CB1,
		PIXEL_MAP_I_TO_I_SIZE:  0x0CB0,
		PIXEL_MAP_I_TO_R_SIZE:  0x0CB2,
		PIXEL_MAP_I_TO_G_SIZE:  0x0CB3,
		PIXEL_MAP_I_TO_B_SIZE:  0x0CB4,
		PIXEL_MAP_I_TO_A_SIZE:  0x0CB5,
		PIXEL_MAP_R_TO_R_SIZE:  0x0CB6,
		PIXEL_MAP_G_TO_G_SIZE:  0x0CB7,
		PIXEL_MAP_B_TO_B_SIZE:  0x0CB8,
		PIXEL_MAP_A_TO_A_SIZE:  0x0CB9,
		PIXEL_MAP_S_TO_S:  0x0C71,
		PIXEL_MAP_I_TO_I:  0x0C70,
		PIXEL_MAP_I_TO_R:  0x0C72,
		PIXEL_MAP_I_TO_G:  0x0C73,
		PIXEL_MAP_I_TO_B:  0x0C74,
		PIXEL_MAP_I_TO_A:  0x0C75,
		PIXEL_MAP_R_TO_R:  0x0C76,
		PIXEL_MAP_G_TO_G:  0x0C77,
		PIXEL_MAP_B_TO_B:  0x0C78,
		PIXEL_MAP_A_TO_A:  0x0C79,
		PACK_ALIGNMENT:  0x0D05,
		PACK_LSB_FIRST:  0x0D01,
		PACK_ROW_LENGTH:  0x0D02,
		PACK_SKIP_PIXELS:  0x0D04,
		PACK_SKIP_ROWS:  0x0D03,
		PACK_SWAP_BYTES:  0x0D00,
		UNPACK_ALIGNMENT:  0x0CF5,
		UNPACK_LSB_FIRST:  0x0CF1,
		UNPACK_ROW_LENGTH:  0x0CF2,
		UNPACK_SKIP_PIXELS:  0x0CF4,
		UNPACK_SKIP_ROWS:  0x0CF3,
		UNPACK_SWAP_BYTES:  0x0CF0,
		ZOOM_X:  0x0D16,
		ZOOM_Y:  0x0D17,

		// Texture mapping
		//

		TEXTURE_ENV:  0x2300,
		TEXTURE_ENV_MODE:  0x2200,
		TEXTURE_1D:  0x0DE0,
		TEXTURE_2D:  0x0DE1,
		TEXTURE_WRAP_S:  0x2802,
		TEXTURE_WRAP_T:  0x2803,
		TEXTURE_MAG_FILTER:  0x2800,
		TEXTURE_MIN_FILTER:  0x2801,
		TEXTURE_ENV_COLOR:  0x2201,
		TEXTURE_GEN_S:  0x0C60,
		TEXTURE_GEN_T:  0x0C61,
		TEXTURE_GEN_MODE:  0x2500,
		TEXTURE_BORDER_COLOR:  0x1004,
		TEXTURE_WIDTH:  0x1000,
		TEXTURE_HEIGHT:  0x1001,
		TEXTURE_BORDER:  0x1005,
		TEXTURE_COMPONENTS:  0x1003,
		NEAREST_MIPMAP_NEAREST:  0x2700,
		NEAREST_MIPMAP_LINEAR:  0x2702,
		LINEAR_MIPMAP_NEAREST:  0x2701,
		LINEAR_MIPMAP_LINEAR:  0x2703,
		OBJECT_LINEAR:  0x2401,
		OBJECT_PLANE:  0x2501,
		EYE_LINEAR:  0x2400,
		EYE_PLANE:  0x2502,
		SPHERE_MAP:  0x2402,
		DECAL:  0x2101,
		MODULATE:  0x2100,
		NEAREST:  0x2600,
		REPEAT:  0x2901,
		CLAMP:  0x2900,
		S:  0x2000,
		T:  0x2001,
		R:  0x2002,
		Q:  0x2003,
		TEXTURE_GEN_R:  0x0C62,
		TEXTURE_GEN_Q:  0x0C63,
		PROXY_TEXTURE_1D:  0x8063,
		PROXY_TEXTURE_2D:  0x8064,
		TEXTURE_PRIORITY:  0x8066,
		TEXTURE_RESIDENT:  0x8067,
		TEXTURE_1D_BINDING:  0x8068,
		TEXTURE_2D_BINDING:  0x8069,

		// Internal texture formats
		//

		ALPHA4:  0x803B,
		ALPHA8:  0x803C,
		ALPHA12:  0x803D,
		ALPHA16:  0x803E,
		LUMINANCE4:  0x803F,
		LUMINANCE8:  0x8040,
		LUMINANCE12:  0x8041,
		LUMINANCE16:  0x8042,
		LUMINANCE4_ALPHA4:  0x8043,
		LUMINANCE6_ALPHA2:  0x8044,
		LUMINANCE8_ALPHA8:  0x8045,
		LUMINANCE12_ALPHA4:  0x8046,
		LUMINANCE12_ALPHA12:  0x8047,
		LUMINANCE16_ALPHA16:  0x8048,
		INTENSITY:  0x8049,
		INTENSITY4:  0x804A,
		INTENSITY8:  0x804B,
		INTENSITY12:  0x804C,
		INTENSITY16:  0x804D,
		R3_G3_B2:  0x2A10,
		RGB4:  0x804F,
		RGB5:  0x8050,
		RGB8:  0x8051,
		RGB10:  0x8052,
		RGB12:  0x8053,
		RGB16:  0x8054,
		RGBA2:  0x8055,
		RGBA4:  0x8056,
		RGB5_A1:  0x8057,
		RGBA8:  0x8058,
		RGB10_A2:  0x8059,
		RGBA12:  0x805A,
		RGBA16:  0x805B,

		// Utility
		//

		VENDOR:  0x1F00,
		RENDERER:  0x1F01,
		VERSION:  0x1F02,
		EXTENSIONS:  0x1F03,

		// Errors
		//

		INVALID_VALUE:  0x0501,
		INVALID_ENUM:  0x0500,
		INVALID_OPERATION:  0x0502,
		STACK_OVERFLOW:  0x0503,
		STACK_UNDERFLOW:  0x0504,
		OUT_OF_MEMORY:  0x0505,

		/*
		 * 1.0 Extensions
		 */

		// GL_EXT_blend_minmax and GL_EXT_blend_color
		//

		CONSTANT_COLOR_EXT:  0x8001,
		ONE_MINUS_CONSTANT_COLOR_EXT:  0x8002,
		CONSTANT_ALPHA_EXT:  0x8003,
		ONE_MINUS_CONSTANT_ALPHA_EXT:  0x8004,
		BLEND_EQUATION_EXT:  0x8009,
		MIN_EXT:  0x8007,
		MAX_EXT:  0x8008,
		FUNC_ADD_EXT:  0x8006,
		FUNC_SUBTRACT_EXT:  0x800A,
		FUNC_REVERSE_SUBTRACT_EXT:  0x800B,
		BLEND_COLOR_EXT:  0x8005,

		// GL_EXT_polygon_offset
		//

		POLYGON_OFFSET_EXT: 0x8037,
		POLYGON_OFFSET_FACTOR_EXT: 0x8038,
		POLYGON_OFFSET_BIAS_EXT: 0x8039,

		// GL_EXT_vertex_array
		//

		VERTEX_ARRAY_EXT:  0x8074,
		NORMAL_ARRAY_EXT:  0x8075,
		COLOR_ARRAY_EXT:  0x8076,
		INDEX_ARRAY_EXT:  0x8077,
		TEXTURE_COORD_ARRAY_EXT:  0x8078,
		EDGE_FLAG_ARRAY_EXT:  0x8079,
		VERTEX_ARRAY_SIZE_EXT:  0x807A,
		VERTEX_ARRAY_TYPE_EXT:  0x807B,
		VERTEX_ARRAY_STRIDE_EXT:  0x807C,
		VERTEX_ARRAY_COUNT_EXT:  0x807D,
		NORMAL_ARRAY_TYPE_EXT:  0x807E,
		NORMAL_ARRAY_STRIDE_EXT:  0x807F,
		NORMAL_ARRAY_COUNT_EXT:  0x8080,
		COLOR_ARRAY_SIZE_EXT:  0x8081,
		COLOR_ARRAY_TYPE_EXT:  0x8082,
		COLOR_ARRAY_STRIDE_EXT:  0x8083,
		COLOR_ARRAY_COUNT_EXT:  0x8084,
		INDEX_ARRAY_TYPE_EXT:  0x8085,
		INDEX_ARRAY_STRIDE_EXT:  0x8086,
		INDEX_ARRAY_COUNT_EXT:  0x8087,
		TEXTURE_COORD_ARRAY_SIZE_EXT:  0x8088,
		TEXTURE_COORD_ARRAY_TYPE_EXT:  0x8089,
		TEXTURE_COORD_ARRAY_STRIDE_EXT: 0x808A,
		TEXTURE_COORD_ARRAY_COUNT_EXT: 0x808B,
		EDGE_FLAG_ARRAY_STRIDE_EXT:  0x808C,
		EDGE_FLAG_ARRAY_COUNT_EXT:  0x808D,
		VERTEX_ARRAY_POINTER_EXT:  0x808E,
		NORMAL_ARRAY_POINTER_EXT:  0x808F,
		COLOR_ARRAY_POINTER_EXT:  0x8090,
		INDEX_ARRAY_POINTER_EXT:  0x8091,
		TEXTURE_COORD_ARRAY_POINTER_EXT: 0x8092,
		EDGE_FLAG_ARRAY_POINTER_EXT:  0x8093, 

		// Occlusion queries
		//

		QUERY_COUNTER_BITS:  0x8864, 
		CURRENT_QUERY:  0x8865, 
		QUERY_RESULT:  0x8866, 
		QUERY_RESULT_AVAILABLE:  0x8867, 
		SAMPLES_PASSED:  0x8914, 

		CURRENT_BIT:  0x00000001,
		POINT_BIT:  0x00000002,
		LINE_BIT:  0x00000004,
		POLYGON_BIT:  0x00000008,
		POLYGON_STIPPLE_BIT:  0x00000010,
		PIXEL_MODE_BIT:  0x00000020,
		LIGHTING_BIT:  0x00000040,
		FOG_BIT:  0x00000080,
		DEPTH_BUFFER_BIT:  0x00000100,
		ACCUM_BUFFER_BIT:  0x00000200,
		STENCIL_BUFFER_BIT:  0x00000400,
		VIEWPORT_BIT:  0x00000800,
		TRANSFORM_BIT:  0x00001000,
		ENABLE_BIT:  0x00002000,
		COLOR_BUFFER_BIT:  0x00004000,
		HINT_BIT:  0x00008000,
		EVAL_BIT:  0x00010000,
		LIST_BIT:  0x00020000,
		TEXTURE_BIT:  0x00040000,
		SCISSOR_BIT:  0x00080000,
		ALL_ATTRIB_BITS:  0x000FFFFF
    });

    function triangleWave(x, period, min, max) {
      const amplitude = max - min;
      const phase = x % period;
      const ascending = 2 * phase / period;
      if (ascending < 1) {
        return min + amplitude * ascending;
      } else {
        return max - amplitude * (ascending - 1);
      }
    }

    function sineWave(x, period, min, max) {
      return (Math.sin(x / period * 3.14) + 1) * 0.5 * (max-min) + min * 2
    }

    function getCounterText(count) { 
      var width = 20
      var position = sineWave(count, 30, 0, width)
      return "lol: " + ('-'.repeat(position) + '#' + '-'.repeat(width - position))
    }; 

)XX";

void drawTriangle(JSContext *ctx) {
  std::string jsCode = R"XX(
    timePassed += 1.0 / 60;

    TinyGL.glMatrixMode(TinyGL.MODELVIEW);
    TinyGL.glLoadIdentity();
    TinyGL.glPushMatrix();
    TinyGL.glRotatef(timePassed * 180, 0, 0, 1);

    TinyGL.glBegin(TinyGL.TRIANGLES);

    (function (s) {
      TinyGL.glColor3f(1, 1, 1);
      TinyGL.glTexCoord2f(0.0, 1.0);
      TinyGL.glVertex3f(-s, -s, s);
      
      TinyGL.glColor3f(1, 1, 1);
      TinyGL.glTexCoord2f(1.0, 1.0);
      TinyGL.glVertex3f(s, -s, s);

      TinyGL.glColor3f(1, 1, 1);
      TinyGL.glTexCoord2f(1.0, 0.0);
      TinyGL.glVertex3f(0, s, s);
    })(0.5);

    TinyGL.glEnd();
    TinyGL.glPopMatrix();
)XX";

  // Evaluate the JavaScript code
  JSValue result = JS_Eval(ctx, jsCode.c_str(), jsCode.length(), "<input>",
                           JS_EVAL_TYPE_GLOBAL);

  JS_FreeValue(ctx, result);
}

void initScene() {

  // glLightfv( GL_LIGHT0, GL_AMBIENT, white);
  // glLightfv( GL_LIGHT0, GL_SPECULAR, white);
  // glEnable( GL_CULL_FACE );
  glDisable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  // glEnable( GL_LIGHT0 );
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glTextSize(GL_TEXT_SIZE24x24);
  /*
  {
          int sw = 0, sh = 0, sc = 0; //sc goes unused.
          uchar* source_data = stbi_load("texture.png", &sw, &sh, &sc, 3);
          if(source_data){
                  tex = loadRGBTexture(source_data, sw, sh);
                  free(source_data);
          } else {
                  printf("\nCan't load texture!\n");
          }
  }*/
  glEnable(GL_NORMALIZE);
}

// Wrapper for glColor3f
static JSValue js_glColor3f(JSContext *ctx, JSValueConst this_val, int argc,
                            JSValueConst *argv) {
  double r, g, b;

  // Extract the arguments from JavaScript
  JS_ToFloat64(ctx, &r, argv[0]);
  JS_ToFloat64(ctx, &g, argv[1]);
  JS_ToFloat64(ctx, &b, argv[2]);

  // Call the actual TinyGL function
  glColor3f((float)r, (float)g, (float)b);

  return JS_UNDEFINED;
}

// Wrapper for glVertex3f
static JSValue js_glVertex3f(JSContext *ctx, JSValueConst this_val, int argc,
                             JSValueConst *argv) {
  double x, y, z;

  // Extract the arguments from JavaScript
  JS_ToFloat64(ctx, &x, argv[0]);
  JS_ToFloat64(ctx, &y, argv[1]);
  JS_ToFloat64(ctx, &z, argv[2]);

  // Call the actual TinyGL function
  glVertex3f((float)x, (float)y, (float)z);

  return JS_UNDEFINED;
}

// Wrapper for glVertex3f
static JSValue js_glTexCoord2f(JSContext *ctx, JSValueConst this_val, int argc,
                               JSValueConst *argv) {
  double x, y;

  // Extract the arguments from JavaScript
  JS_ToFloat64(ctx, &x, argv[0]);
  JS_ToFloat64(ctx, &y, argv[1]);

  // Call the actual TinyGL function
  glTexCoord2f((float)x, (float)y);

  return JS_UNDEFINED;
}

// Wrapper for glMatrixMode
static JSValue js_glMatrixMode(JSContext *ctx, JSValueConst this_val, int argc,
                               JSValueConst *argv) {
  int mode;
  JS_ToInt32(ctx, &mode, argv[0]);
  glMatrixMode(mode);
  return JS_UNDEFINED;
}

// Wrapper for glLoadIdentity
static JSValue js_glLoadIdentity(JSContext *ctx, JSValueConst this_val,
                                 int argc, JSValueConst *argv) {
  glLoadIdentity();
  return JS_UNDEFINED;
}

// Wrapper for glPushMatrix
static JSValue js_glPushMatrix(JSContext *ctx, JSValueConst this_val, int argc,
                               JSValueConst *argv) {
  glPushMatrix();
  return JS_UNDEFINED;
}

// Wrapper for glRotatef
static JSValue js_glRotatef(JSContext *ctx, JSValueConst this_val, int argc,
                            JSValueConst *argv) {
  double angle, x, y, z;
  JS_ToFloat64(ctx, &angle, argv[0]);
  JS_ToFloat64(ctx, &x, argv[1]);
  JS_ToFloat64(ctx, &y, argv[2]);
  JS_ToFloat64(ctx, &z, argv[3]);
  glRotatef((float)angle, (float)x, (float)y, (float)z);
  return JS_UNDEFINED;
}

// Wrapper for glBegin
static JSValue js_glBegin(JSContext *ctx, JSValueConst this_val, int argc,
                          JSValueConst *argv) {
  int mode;
  JS_ToInt32(ctx, &mode, argv[0]);
  glBegin(mode);
  return JS_UNDEFINED;
}

// Wrapper for glEnd
static JSValue js_glEnd(JSContext *ctx, JSValueConst this_val, int argc,
                        JSValueConst *argv) {
  glEnd();
  return JS_UNDEFINED;
}

// Wrapper for glPopMatrix
static JSValue js_glPopMatrix(JSContext *ctx, JSValueConst this_val, int argc,
                              JSValueConst *argv) {
  glPopMatrix();
  return JS_UNDEFINED;
}

void js_tinygl_init(JSContext *ctx, JSValue ns) {
  // Register each function manually
  JS_SetPropertyStr(ctx, ns, "glColor3f",
                    JS_NewCFunction(ctx, js_glColor3f, "glColor3f", 3));
  JS_SetPropertyStr(ctx, ns, "glVertex3f",
                    JS_NewCFunction(ctx, js_glVertex3f, "glVertex3f", 3));
  JS_SetPropertyStr(ctx, ns, "glTexCoord2f",
                    JS_NewCFunction(ctx, js_glTexCoord2f, "glTexCoord2f", 2));
  JS_SetPropertyStr(ctx, ns, "glMatrixMode",
                    JS_NewCFunction(ctx, js_glMatrixMode, "glMatrixMode", 1));
  JS_SetPropertyStr(
      ctx, ns, "glLoadIdentity",
      JS_NewCFunction(ctx, js_glLoadIdentity, "glLoadIdentity", 0));

  JS_SetPropertyStr(ctx, ns, "glPushMatrix",
                    JS_NewCFunction(ctx, js_glPushMatrix, "glPushMatrix", 0));
  JS_SetPropertyStr(ctx, ns, "glRotatef",
                    JS_NewCFunction(ctx, js_glRotatef, "glRotatef", 4));
  JS_SetPropertyStr(ctx, ns, "glBegin",
                    JS_NewCFunction(ctx, js_glBegin, "glBegin", 1));
  JS_SetPropertyStr(ctx, ns, "glEnd",
                    JS_NewCFunction(ctx, js_glEnd, "glEnd", 0));
  JS_SetPropertyStr(ctx, ns, "glPopMatrix",
                    JS_NewCFunction(ctx, js_glPopMatrix, "glPopMatrix", 0));
}

std::string getCounterText(JSContext *ctx, int frameCount) {
  std::string jsCode = "getCounterText(" + std::to_string(frameCount) + ");";

  // Evaluate the JavaScript code
  JSValue result = JS_Eval(ctx, jsCode.c_str(), jsCode.length(), "<input>",
                           JS_EVAL_TYPE_GLOBAL);

  // Convert the result to a C string
  const char *resultStr = JS_ToCString(ctx, result);
  if (!resultStr) {
    JS_FreeValue(ctx, result);
    return "JS Error";
  }

  // Convert the result to a std::string and free the JS string
  std::string counterText(resultStr);
  JS_FreeCString(ctx, resultStr);
  JS_FreeValue(ctx, result);

  return counterText;
}

cv::Mat resizeAndCropToSquare(cv::Mat img, int outputSize) {
  // Step 1: Resize image maintaining aspect ratio
  double scale = outputSize / static_cast<double>(std::min(img.cols, img.rows));
  cv::Mat resized;
  cv::resize(img, resized, cv::Size(), scale, scale, cv::INTER_LINEAR);

  // Step 2: Crop the center of the image
  int cropStartX = (resized.cols - outputSize) / 2;
  int cropStartY = (resized.rows - outputSize) / 2;
  cv::Rect roi(cropStartX, cropStartY, outputSize,
               outputSize);       // Define a rectangle for cropping
  cv::Mat cropped = resized(roi); // Crop the image

  return cropped;
}

int main() {
  std::cout << "Hello, CMake World!" << std::endl;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  // Initialize OpenCV capture
  cv::VideoCapture cap(0); // Open the default camera
  if (!cap.isOpened()) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't open camera");
    return 1;
  }

  cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
  // cap.set(cv::CAP_PROP_FRAME_HEIGHT, 120);

  // Check what resolution was actually set
  double width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  double height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
  std::cout << "Resolution set to: " << width << "x" << height << std::endl;

  cv::Mat frame;
  SDL_Texture *cameraTexture = nullptr;

  SDL_Window *window = SDL_CreateWindow("QuickGL", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 480, 480, 0);

  if (!window) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return 3;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    return 4;
  }

  SDL_Texture *albedoTexture = SDL_CreateTexture(
      renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, 480, 480);

  // Initialize TinyGL
  ZBuffer *zbuffer = ZB_open(480, 480, ZB_MODE_5R6G5B, NULL);

  if (!zbuffer) {
    SDL_Log("Failed to create zbuffer");
    return 3;
  }

  glInit(zbuffer);

  // QuickJS

  // Initialize the QuickJS runtime and context
  JSRuntime *rt = JS_NewRuntime();
  JSContext *ctx = JS_NewContext(rt);

  JSValue tinyglNamespace = JS_NewObject(ctx);
  js_tinygl_init(ctx, tinyglNamespace);
  JS_SetPropertyStr(ctx, JS_GetGlobalObject(ctx), "TinyGL", tinyglNamespace);

  // Evaluate the JavaScript code
  JSValue result = JS_Eval(ctx, templateCode.c_str(), templateCode.length(),
                           "<input>", JS_EVAL_TYPE_GLOBAL);
  JS_FreeValue(ctx, result);

  // Render...

  bool running = true;
  SDL_Event event;

  int frameCount = 0;

  // Load face cascade
  face_cascade.load("../data/haarcascade_frontalface_default.xml");

  while (running) {
    cap >> frame; // Capture a frame

    auto start = std::chrono::high_resolution_clock::now();

    // Convert frame to a tinygl texture
    cv::Mat frameRGB;
    cv::cvtColor(frame, frameRGB, cv::COLOR_BGR2RGB);

    cv::Mat crop = resizeAndCropToSquare(frameRGB, 480);
    cv::Mat temp;
    crop.convertTo(temp, CV_8U);

    /*
    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(
        temp, faces, 1.2, 5, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(80, 80));

    // Here you can use the positions of the faces for whatever you need
    for (size_t i = 0; i < faces.size(); i++) {
      cv::Point center(faces[i].x + faces[i].width * 0.5,
                       faces[i].y + faces[i].height * 0.5);
      ellipse(temp, center,
              cv::Size(faces[i].width * 0.5, faces[i].height * 0.5), 0, 0, 360,
              cv::Scalar(255, 0, 255), 4, 8, 0);
    }
    */

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLuint t = 0;
    glGenTextures(1, &t);
    glBindTexture(GL_TEXTURE_2D, t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 480, 480, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 (unsigned char *)temp.data);

    // Draw background
    glBegin(GL_QUADS);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, t);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-1, -1); // Bottom Left
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(1, -1); // Bottom Right
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(1, 1); // Top Right
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-1, 1); // Top Left
    glEnd();

    drawTriangle(ctx);

    // Set text to render
    std::string counterText = getCounterText(ctx, frameCount++);
    glDrawText((unsigned char *)counterText.c_str(), 34, 32, 0x313131);
    glDrawText((unsigned char *)counterText.c_str(), 32, 30, 0xfafafa);

    // End time and calculate duration
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::ostringstream oss;
    oss << "Frame time: " << duration.count() << " milliseconds." << std::endl;
    std::string frameText = oss.str();

    glDrawText((unsigned char *)frameText.c_str(), 34, 50, 0x313131);
    glDrawText((unsigned char *)frameText.c_str(), 32, 48, 0xc0c0c0);

    // Copy the TinyGL framebuffer to the SDL texture
    void *pixels;
    int pitch;

    SDL_LockTexture(albedoTexture, NULL, &pixels, &pitch);
    memcpy(pixels, zbuffer->pbuf, 480 * 480 * 2);
    SDL_UnlockTexture(albedoTexture);

    SDL_RenderCopy(renderer, albedoTexture, NULL, NULL);

    // Update screen
    SDL_RenderPresent(renderer);

    // Free resources
    // SDL_FreeSurface(textSurface);
    // SDL_DestroyTexture(textTexture);

    // SDL_Delay(1000 / 60); // Approximately 60 frames per second

    // time_passed += 1.0 / 60;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }
  }

  // Cleanup SDL
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  // Free the result value
  JS_FreeValue(ctx, result);

  // Perform cleanup
  JS_FreeContext(ctx);

  return 0;
}
