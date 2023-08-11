# ifndef ARR_PERM_H
# define ARR_PERM_H

# include <iostream>
# include <vector>
# include <string>

std::string ALFABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

struct arr_p {

	unsigned int waarden[26];

	// id
	arr_p() {
		for (int i = 0; i < 26; i++) {
			waarden[i] = i;
		}
	}

	arr_p(std::string woord) {
		for (int i = 0; i < 26; i++) {
			waarden[i] = woord[i] - 'A';
		}
	}

	void operator= (arr_p sigma) {
		for (int i = 0; i < 26; i++) {
			waarden[i] = sigma(i);
		}
	}

	int operator() (unsigned int k) {
		return waarden[k];
	}

	arr_p operator* (arr_p tau) {
		arr_p res;
		for (int i = 0; i < 26; i++) {
			res.waarden[i] = waarden[tau(i)];
		}

		return res;
	}

	void druk_af() {
		for (int i = 0; i < 26; i++) {
			if (i < 10) {
				std::cout << i << "  ";
			}
			else {
				std::cout << i << " ";
			}
		}
		std::cout << std::endl;
		for (int i = 0; i < 26; i++) {
			if (waarden[i] < 10) {
				std::cout << waarden[i] << "  ";
			}
			else {
				std::cout << waarden[i] << " ";
			}
		}
		std::cout << std::endl;
	}

	arr_p inverse() {
		arr_p inv;
		for (int i = 0; i < 26; i++) {
			inv.waarden[waarden[i]] = i;
		}
		return inv;
	}

	arr_p conjugeer(arr_p sigma) {
		arr_p inv;
		for (int i = 0; i < 26; i++) {
			inv.waarden[sigma(i)] = i;
		}

		arr_p res;
		for (int i = 0; i < 26; i++) {
			res.waarden[i] = inv(waarden[sigma(i)]);
		}

		return res;
	}

	arr_p maak_epsilon(arr_p rho0, arr_p rho1, arr_p rho2, int s, int m, int t) {
		arr_p spr;
		for (int i = 0; i < 26; i++) {
			spr.waarden[i] = (rho2((rho1((rho0((i + s) % 26) + m - s + 26) % 26) + t - m + 26) % 26) + 26 - t) % 26;
		}

		return conjugeer(spr);
	}

};

arr_p arr_macht_pi(int k) {
		arr_p res;
		for (int i = 0; i < 26; i++) {
			res.waarden[i] = (k + i + 26) % 26;
		}

		return res;
	}

# endif
