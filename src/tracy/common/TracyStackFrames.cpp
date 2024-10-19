#include "TracyStackFrames.hpp"

namespace tracy {

	const char* s_tracyStackFrames_[] = {
		"tracy::Callstack",
		"tracy::Callstack(int)",
		"tracy::GpuCtxScope::{ctor}",
		"tracy::Profiler::SendCallstack",
		"tracy::Profiler::SendCallstack(int)",
		"tracy::Profiler::SendCallstack(int, unsigned long)",
		"tracy::Profiler::MemAllocCallstack",
		"tracy::Profiler::MemAllocCallstack(void const*, unsigned long, int)",
		"tracy::Profiler::MemFreeCallstack",
		"tracy::Profiler::MemFreeCallstack(void const*, int)",
		"tracy::ScopedZone::{ctor}",
		"tracy::ScopedZone::ScopedZone(tracy::SourceLocationData const*, int, bool)",
		"tracy::Profiler::Message",
		nullptr};

	const char** s_tracyStackFrames = s_tracyStackFrames_;

	const StringMatch s_tracySkipSubframes_[] = {
		{"/include/arm_neon.h", 19},
		{"/include/adxintrin.h", 20},
		{"/include/ammintrin.h", 20},
		{"/include/amxbf16intrin.h", 24},
		{"/include/amxint8intrin.h", 24},
		{"/include/amxtileintrin.h", 24},
		{"/include/avx2intrin.h", 21},
		{"/include/avx5124fmapsintrin.h", 29},
		{"/include/avx5124vnniwintrin.h", 29},
		{"/include/avx512bf16intrin.h", 27},
		{"/include/avx512bf16vlintrin.h", 29},
		{"/include/avx512bitalgintrin.h", 29},
		{"/include/avx512bwintrin.h", 25},
		{"/include/avx512cdintrin.h", 25},
		{"/include/avx512dqintrin.h", 25},
		{"/include/avx512erintrin.h", 25},
		{"/include/avx512fintrin.h", 24},
		{"/include/avx512ifmaintrin.h", 27},
		{"/include/avx512ifmavlintrin.h", 29},
		{"/include/avx512pfintrin.h", 25},
		{"/include/avx512vbmi2intrin.h", 28},
		{"/include/avx512vbmi2vlintrin.h", 30},
		{"/include/avx512vbmiintrin.h", 27},
		{"/include/avx512vbmivlintrin.h", 29},
		{"/include/avx512vlbwintrin.h", 27},
		{"/include/avx512vldqintrin.h", 27},
		{"/include/avx512vlintrin.h", 25},
		{"/include/avx512vnniintrin.h", 27},
		{"/include/avx512vnnivlintrin.h", 29},
		{"/include/avx512vp2intersectintrin.h", 35},
		{"/include/avx512vp2intersectvlintrin.h", 37},
		{"/include/avx512vpopcntdqintrin.h", 32},
		{"/include/avx512vpopcntdqvlintrin.h", 34},
		{"/include/avxintrin.h", 20},
		{"/include/avxvnniintrin.h", 24},
		{"/include/bmi2intrin.h", 21},
		{"/include/bmiintrin.h", 20},
		{"/include/bmmintrin.h", 20},
		{"/include/cetintrin.h", 20},
		{"/include/cldemoteintrin.h", 25},
		{"/include/clflushoptintrin.h", 27},
		{"/include/clwbintrin.h", 21},
		{"/include/clzerointrin.h", 23},
		{"/include/emmintrin.h", 20},
		{"/include/enqcmdintrin.h", 23},
		{"/include/f16cintrin.h", 21},
		{"/include/fma4intrin.h", 21},
		{"/include/fmaintrin.h", 20},
		{"/include/fxsrintrin.h", 21},
		{"/include/gfniintrin.h", 21},
		{"/include/hresetintrin.h", 23},
		{"/include/ia32intrin.h", 21},
		{"/include/immintrin.h", 20},
		{"/include/keylockerintrin.h", 26},
		{"/include/lwpintrin.h", 20},
		{"/include/lzcntintrin.h", 22},
		{"/include/mmintrin.h", 19},
		{"/include/movdirintrin.h", 23},
		{"/include/mwaitxintrin.h", 23},
		{"/include/nmmintrin.h", 20},
		{"/include/pconfigintrin.h", 24},
		{"/include/pkuintrin.h", 20},
		{"/include/pmmintrin.h", 20},
		{"/include/popcntintrin.h", 23},
		{"/include/prfchwintrin.h", 23},
		{"/include/rdseedintrin.h", 23},
		{"/include/rtmintrin.h", 20},
		{"/include/serializeintrin.h", 26},
		{"/include/sgxintrin.h", 20},
		{"/include/shaintrin.h", 20},
		{"/include/smmintrin.h", 20},
		{"/include/tbmintrin.h", 20},
		{"/include/tmmintrin.h", 20},
		{"/include/tsxldtrkintrin.h", 25},
		{"/include/uintrintrin.h", 22},
		{"/include/vaesintrin.h", 21},
		{"/include/vpclmulqdqintrin.h", 27},
		{"/include/waitpkgintrin.h", 24},
		{"/include/wbnoinvdintrin.h", 25},
		{"/include/wmmintrin.h", 20},
		{"/include/x86gprintrin.h", 23},
		{"/include/x86intrin.h", 20},
		{"/include/xmmintrin.h", 20},
		{"/include/xopintrin.h", 20},
		{"/include/xsavecintrin.h", 23},
		{"/include/xsaveintrin.h", 22},
		{"/include/xsaveoptintrin.h", 25},
		{"/include/xsavesintrin.h", 23},
		{"/include/xtestintrin.h", 22},
		{"/bits/atomic_base.h", 19},
		{"/atomic", 7},
		{}};

	const StringMatch* s_tracySkipSubframes = s_tracySkipSubframes_;

} // namespace tracy
