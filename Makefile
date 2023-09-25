.PHONY: test
test:
	[ ! -d build ] && cmake -B build || true

	cd build \
		&& make \
		&& ctest --output-on-failure
