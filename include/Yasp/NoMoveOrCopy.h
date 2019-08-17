#ifndef YASP_NO_MOVE_OR_COPY
#define YASP_NO_MOVE_OR_COPY

namespace yasp
{
	class NoMoveOrCopy
	{
	public:
		NoMoveOrCopy() = default;
		~NoMoveOrCopy() = default;
		NoMoveOrCopy(const NoMoveOrCopy&) = delete;
		NoMoveOrCopy& operator=(const NoMoveOrCopy&) = delete;
		NoMoveOrCopy(NoMoveOrCopy&&) = delete;
		NoMoveOrCopy& operator=(NoMoveOrCopy&&) = delete;
	};
}

#endif
