
# include "enigma.h"

int main() {
	
	// gegeven
	arr_p rho0("AJDKSIRUXBLHWTMCQGZNPYFVOE");
	arr_p rho1("EKMFLGDQVZNTOWYHXUSPAIBRCJ");
	arr_p rho2("BDFHJLCPRTXVZNYEIWGAKMUSQO");
	arr_p rho3("THEQUICKBROWNFXJMPSVLAZYDG");
	arr_p rho4("XANTIPESOKRWUDVBCFGHJLMQYZ");

	arr_p alle_rotoren[5] = { rho0, rho1, rho2, rho3, rho4 };

	arr_p tau("YRUHQSLDPXNGOKMIEBFZCWVJAT");

	std::string crib = "DEOPGAVEVOORENIGMA";
	std::string beeld = "LXCACYLPNSIZCOQMAZ";


	// uitvoeren van deze code levert de rotoren, de posities en de stekkerbordpermutatie
	// arr_bomba(alle_rotoren, tau, crib, beeld);

	
	// resultaat
	arr_p rotoren[3] = { rho2, rho3, rho1 };
	char posities[3] = { 'J', 'E', 'F' };
	arr_p sigma("AYCWEUGSIQKOMNLPJRHTFVDXBZ");


	// ontcijferen van de tekst
	std::string tekst = bestand_enigma("versleutelde tekst.txt", "ontcijferde tekst.txt", sigma, tau, rotoren, posities);


	// controle
	std::cout << "De crib: " << tekst.substr(0, 18) << std::endl << "Het begin van de tekst: " << tekst.substr(18, 50) << std::endl;

	return 0;
}
