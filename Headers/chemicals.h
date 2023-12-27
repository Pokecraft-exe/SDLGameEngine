#pragma once

struct properties {
	bool flamable;
	int k_min;
	int pas_min;
	int p;
	int n;
	int e;
};

enum elements {
	Hydrogene,
	Helium,
	Lithium,
	Berylium,
	Bore,
	Azote,
	Carbonne,
	Oxygene,
	Fluore,
	Neon
};

struct particle {
	size_t size;
	properties property_;
	elements* atoms;
};

class chemicals
{
	void reaction();
};

