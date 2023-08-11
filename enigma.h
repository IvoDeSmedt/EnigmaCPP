# ifndef ENIGMA_H
# define ENIGMA_H

# include <algorithm>
# include <fstream>
# include "arr_perm.h"

void voeg_edge_toe(bool**& matrix, int grootte, int rij1, int kolom1, int rij2, int kolom2) {
    matrix[grootte * rij1 + kolom1][grootte * rij2 + kolom2] = 1;
    matrix[grootte * rij2 + kolom2][grootte * rij1 + kolom1] = 1;
}

void wandeling(bool**& matrix, bool**& graaf, int grootte, int rij, int kolom) {

    // kom aan in de vertex (rij,kolom) en vink deze af als zijnde bezocht
    graaf[rij][kolom] = true;
    std::vector<int> te_bezoeken = {};

    // zoek in de naburigheidsmatrix naar vertices die met (rij,kolom) verbonden zijn
    // voeg ze toe aan de lijst 'te_bezoeken'
    for (int i = 0; i < grootte; i++) {
        for (int j = 0; j < grootte; j++) {
            if (matrix[grootte * rij + kolom][grootte * i + j] && !graaf[i][j]) {
                te_bezoeken.push_back(grootte * i + j);
            }
        }
    }

    // wandel naar elke vertex die nog bezocht moest worden
    for (int index : te_bezoeken) {
        wandeling(matrix, graaf, grootte, index / grootte, index % grootte);
    }
}

std::string arr_enigma(arr_p stekkerbord, arr_p reflector, arr_p* rotoren, char* posities, std::string const& tekst) {
    
    int N = tekst.size();

    std::string resultaat("", N);

    int s = posities[0] - 'A';
    int m = posities[1] - 'A';
    int t = posities[2] - 'A';

    for (int k = 0; k < N; k++) {

        int l = tekst[k] - 'A';

        l = stekkerbord(l);
        
        l = (26 + l + s) % 26;

        l = rotoren[0](l);
        
        l = (26 + l + m - s) % 26;

        l = rotoren[1](l);

        l = (26 + l - m + t) % 26;

        l = rotoren[2](l);

        l = (26 + l - t) % 26;

        l = reflector(l);

        l = (26 + l + t) % 26;

        l = rotoren[2].inverse()(l);
        
        l = (26 + l + m - t) % 26;
        
        l = rotoren[1].inverse()(l);
        
        l = (26 + l + s - m) % 26;
        
        l = rotoren[0].inverse()(l);
        
        l = (26 + l - s) % 26;

        l = stekkerbord(l);

        resultaat[k] = ALFABET[l];

        s++;

        if (s % 26 == 0) {
            s = 0;
            m++;
        }

        if (m % 26 == 0 && m != 0) {
            m = 0;
            t++;
        }

        if (t % 26 == 0 && t != 0) {
            return resultaat;
        }

    }

    return resultaat;

}

std::string bestand_enigma(std::string lezen, std::string schrijven, arr_p stekkerbord, arr_p reflector, arr_p* rotoren, char* posities) {
    std::fstream bestand_lezen, bestand_schrijven;
    bestand_lezen.open(lezen, std::ios::in);
    bestand_schrijven.open(schrijven, std::ios::out);

    std::string resultaat = "";

    if (bestand_lezen.good()) {
        if (bestand_lezen.is_open()) {
            std::string lijn;
            while (std::getline(bestand_lezen, lijn)) {
                resultaat += arr_enigma(stekkerbord, reflector, rotoren, posities, lijn);
                bestand_schrijven << resultaat;
            }
        }
    }
    else {
        std::cout << "Fout: bestand niet kunnen vinden." << std::endl;
    }

    return resultaat;

}

void arr_bomba(arr_p* rotoren, arr_p reflector, std::string crib, std::string beeld) {
    // Maak de matrix van de variabele graaf
    bool** GAMMA = new bool* [26 * 26];
    for (int i = 0; i < 26 * 26; i++) {
        GAMMA[i] = new bool[26 * 26]();
    }

    // maak de variabele graaf aan
    bool** graaf = new bool* [26];
    for (int i = 0; i < 26; i++) {
        graaf[i] = new bool[26]();
    }

    std::vector<int> P(26, -1);

    // lopen over alle rotorcombinaties
    int q = 0;
    std::vector<int> V = { 0, 1, 2, 3, 4 };
    std::cout << "De rotoren en rotorposities staan altijd in de volgorde: snel - midden - traag." << std::endl << std::endl;
    do {
        // de rotorstand afdrukken en de rotoren kiezen
        std::cout << std::endl << "combinatie: " << q + 1 << ", rotoren: " << V[0] << " - " << V[1] << " - " << V[2] << std::endl;

        // lopen over alle k
        for (int k = 0; k < 17576; k++) {

            if (k != 3493) {
                // continue;
            }

            // verbind de vertices (i,j) met (j,i) wanneer i en j niet gelijk zijn
            for (int i = 0; i < 676; i++) {
                for (int j = 0; j < 676; j++) {
                    GAMMA[i][j] = (i / 26 == j % 26) && (i % 26 == j / 26) && (i != j);
                }
            }

            // lopen over de crib(graaf)
            for (int l = 0; l < 18; l++) {

                char L1 = crib[l];
                char L2 = beeld[l];

                // de indices van L1 en L2 in het alfabet
                int rij1 = L1 - 'A';
                int rij2 = L2 - 'A';

                // de machten van pi voor elk van de rotoren bepalen (in functie van het gewicht in de cribgraaf)
                int H = (k + l) % 17576;
                int s = H % 26;
                int m = (H / 26) % 26;
                int t = H / 676;

                arr_p epsilon = reflector.maak_epsilon(rotoren[V[0]], rotoren[V[1]], rotoren[V[2]], s, m, t);

                // de hele rij van L1 afbeelden
                for (int kolom1 = 0; kolom1 < 26; kolom1++) {

                    int kolom2 = epsilon(kolom1);
                    voeg_edge_toe(GAMMA, 26, rij1, kolom1, rij2, kolom2);
                }
            }


            // vul de variabele graaf met nullen
            for (int i = 0; i < 26; i++) {
                for (int j = 0; j < 26; j++) {
                    graaf[i][j] = false;
                }
            }

            // loop over GAMMA, beginnende bij de vertex (14,11) = (O,L)
            // de cribgraaf is samenhangend, dus kunnen we eenderwelke vertex kiezen die in de cribgraaf ligt
            wandeling(GAMMA, graaf, 26, 14, 11);


            // is dit de juiste k?

            std::vector<int> rijen(26, 0);
            std::vector<int> kolommen(26, 0);

            // tel het aantal enen in elke rij en kolom
            for (int i = 0; i < 26; i++) {
                for (int j = 0; j < 26; j++) {
                    if (graaf[i][j]) {
                        rijen[i]++;
                        kolommen[j]++;
                    }
                }
            }

            bool permutatiematrix = true;

            // de waarde van k is niet goed als er een rij of kolom is waarvan het aantal enen niet 1 of 25 is
            // we staan minder dan 5 en meer dan 21 enen toe, anders krijgen we geen resultaat
            for (int i = 0; i < 26; i++) {
                if (rijen[i] > 5 && rijen[i] < 21) {
                    permutatiematrix = false;
                    break;
                }
                if (kolommen[i] > 5 && kolommen[i] < 21) {
                    permutatiematrix = false;
                    break;
                }
                if (rijen[i] == 26 || kolommen[i] == 26) {
                    permutatiematrix = false;
                    break;
                }
            }
            if (permutatiematrix) {
                std::cout << std::endl << "Gevonden: k = " << k << std::endl;
                std::cout << "snel: " << V[0] << " in positie " << ALFABET[k % 26] << ",   midden: " << V[1] << " in positie " << ALFABET[(k / 26) % 26] << ",   traag : " << V[2] << " in positie " << ALFABET[k / (26 * 26)] << std::endl;
                for (int i = 0; i < 26; i++) {
                    for (int j = 0; j < 26; j++) {
                        std::cout << graaf[i][j] << " ";
                        if (graaf[i][j]) {
                            P[i] = j;
                        }
                    }
                    std::cout << std::endl;
                }
                return;
            }

        }
        std::cout << std::endl;

        q++;
        std::reverse(V.begin() + 3, V.end());
    } while (next_permutation(V.begin(), V.end()));

    // verwijder de variabele graaf
    for (int i = 0; i < 26; i++) {
        delete[] graaf[i];
    }
    delete[] graaf;

    // verwijder de matrix
    for (int i = 0; i < 26 * 26; i++) {
        delete[] GAMMA[i];
    }
    delete[] GAMMA;

}

# endif
