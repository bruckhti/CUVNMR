/*
* Copyright (c) 2026 Tim Bruckhoff / Affiliation: Ruprecht-Karls-University Heidelberg, Germany
*
* This source code is licensed under the EUPL v1.2.
* See the LICENSE file in the repository root directory for full details.
*
* CITATION:
* If you use this software for your research, please cite it as
* described in the CITATION.cff file located in the root directory.
*
* SPDX-License-Identifier: EUPL-1.2
*/

#include "pch.h"
#include "physical_constants.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

double N_avo;
double mu0;
double g_e;
double muB;
double k_B;
double h;
double hquer;
double PI;
double a0;
double Aprefac;
std::vector<double> gamma;

void initialize_physical_constants()
{
    N_avo = 6.02214076 * pow(10.0, 23);
    mu0 = 1.2566370621219 * pow(10, -6);
    g_e = 2.0023193043625635;
    muB = 9.274010078328 * pow(10, -24);
    k_B = 1.380649 * pow(10, -23);
    h = 6.62607015 * pow(10, -34);
    PI = 3.14159265358979323846;
    hquer = h / (2 * PI);
    a0 = 5.29E-11;
    double epsilon0 = 8.854187818814 * pow(10, -12);
    double e = -1.602176634 * pow(10, -19);
    double me = 9.109383713928 * pow(10, -31);
    a0 = (4.0 * PI * epsilon0 * hquer * hquer) / (e * e * me);
    Aprefac = - (1.0 / 8.0) * (mu0 / (PI * PI * a0 * a0 * a0))* hquer;

//Refs: CODATA 2018, CODATA 2010, NMR Enc. 1996, http://dx.doi.org/10.1103/PhysRevC.63.037301
//case 'E'  // Electron
    gamma.push_back(-1.76085963023e11);
//case 'N'  // Neutron
    gamma.push_back(-1.83247171e8);
//case 'M'  // Muon
    gamma.push_back(-8.51615503e8);
//case 'P'  // Proton
    gamma.push_back(2.6752218744e8);
//case '1H'
    gamma.push_back(2.6752218744e8);
//case '2H'
    gamma.push_back(4.10662791e7);
//case '3H'
    gamma.push_back(2.85349779e8);
//case '3He'
    gamma.push_back(-2.03801587e8);
//case '4He'
    gamma.push_back(0);
//case '6Li'
    gamma.push_back(3.9371709e7);
//case '7Li'
    gamma.push_back(1.03977013e8);
//case '9Be'
    gamma.push_back(-3.759666e7);
//case '10B'
    gamma.push_back(2.8746786e7);
//case '11B'
    gamma.push_back(8.5847044e7);
//case '12C'       
    gamma.push_back(0);
//case '13C'
    gamma.push_back(6.728284e7);
//case '14N'
    gamma.push_back(1.9337792e7);
//case '15N'
    gamma.push_back(-2.71261804e7);
//case '16O'    
    gamma.push_back(0);
//case '17O'
    gamma.push_back(-3.62808e7);
//case '18O'     
    gamma.push_back(0);
//case '19F'
    gamma.push_back(2.518148e8);
//case '20Ne'       
    gamma.push_back(0);
//case '21Ne'
    gamma.push_back(-2.11308e7);
//case '22Ne'    
    gamma.push_back(0);
//case '23Na'
    gamma.push_back(7.0808493e7);
//case '24Mg'      
    gamma.push_back(0);
//case '25Mg'
    gamma.push_back(-1.63887e7);
//case '26Mg'     
    gamma.push_back(0);
//case '27Al'
    gamma.push_back(6.9762715e7);
//case '28Si'       
    gamma.push_back(0);
//case '29Si'
    gamma.push_back(-5.3190e7);
//case '30Si'       
    gamma.push_back(0);
//case '31P'
    gamma.push_back(10.8394e7);
//case '32S'     
    gamma.push_back(0);
//case '33S'
    gamma.push_back(2.055685e7);
//case '34S'     
    gamma.push_back(0);
//case '36S'        
    gamma.push_back(0);
//case '35Cl'
    gamma.push_back(2.624198e7);
//case '37Cl'
    gamma.push_back(2.184368e7);
//case '36Ar'        
    gamma.push_back(0);
//case '38Ar'      
    gamma.push_back(0);
//case '39Ar'
    gamma.push_back(-1.78e7);
//case '40Ar'       
    gamma.push_back(0);
//case '39K'
    gamma.push_back(1.2500608e7);
//case '40K'
    gamma.push_back(-1.5542854e7);
//case '41K'
    gamma.push_back(0.68606808e7);
//case '40Ca'       
    gamma.push_back(0);
//case '41Ca'
    gamma.push_back(-2.182305e7);
//case '42Ca'       
    gamma.push_back(0);
//case '43Ca'
    gamma.push_back(-1.803069e7);
//case '44Ca'       
    gamma.push_back(0);
//case '45Sc'
    gamma.push_back(6.5087973e7);
//case '46Ti'       
    gamma.push_back(0);
//case '47Ti'
    gamma.push_back(-1.5105e7);
//case '48Ti'       
    gamma.push_back(0);
//case '49Ti'
    gamma.push_back(-1.51095e7);
//case '50Ti'      
    gamma.push_back(0);
//case '50V'
    gamma.push_back(2.6706490e7);
//case '51V'
    gamma.push_back(7.0455117e7);
//case '52Cr'       
    gamma.push_back(0);
//case '53Cr'
    gamma.push_back(-1.5152e7);
//case '54Cr'       
    gamma.push_back(0);
//case '55Mn'
    gamma.push_back(6.6452546e7);
//case '54Fe'      
    gamma.push_back(0);
//case '56Fe'       
    gamma.push_back(0);
//case '57Fe'
    gamma.push_back(0.8680624e7);
//case '58Fe'       
    gamma.push_back(0);
//case '59Co'
    gamma.push_back(6.332e7);
//case '60Ni'       
    gamma.push_back(0);
//case '61Ni'
    gamma.push_back(-2.3948e7);
//case '62Ni'      
    gamma.push_back(0);
//case '64Ni'        
    gamma.push_back(0);
//case '63Cu'
    gamma.push_back(7.1117890e7);
//case '65Cu'
    gamma.push_back(7.60435e7);
//case '64Zn'        
    gamma.push_back(0);
//case '66Zn'        
    gamma.push_back(0);
//case '67Zn'
    gamma.push_back(1.676688e7);
//case '68Zn'      
    gamma.push_back(0);
//case '69Ga'
    gamma.push_back(6.438855e7);
//case '71Ga'
    gamma.push_back(8.181171e7);
//case '70Ge'      
    gamma.push_back(0);
//case '72Ge'       
    gamma.push_back(0);
//case '73Ge'
    gamma.push_back(-0.9722881e7);
//case '74Ge'     
    gamma.push_back(0);
//case '75As'
    gamma.push_back(4.596163e7);
//case '74Se'       
    gamma.push_back(0);
//case '76Se'       
    gamma.push_back(0);
//case '77Se'
    gamma.push_back(5.1253857e7);
//case '78Se'       
    gamma.push_back(0);
//case '79Br'
    gamma.push_back(6.725616e7);
//case '81Br'
    gamma.push_back(7.249776e7);
//case '80Kr'      
    gamma.push_back(0);
//case '82Kr'       
    gamma.push_back(0);
//case '83Kr'
    gamma.push_back(-1.03310e7);
//case '84Kr'      
    gamma.push_back(0);
//case '86Kr'        
    gamma.push_back(0);
//case '85Rb'
    gamma.push_back(2.5927050e7);
//case '87Rb'
    gamma.push_back(8.786400e7);
//case '84Sr'      
    gamma.push_back(0);
//case '86Sr'      
    gamma.push_back(0);
//case '87Sr'
    gamma.push_back(-1.1639376e7);
//case '88Sr'      
    gamma.push_back(0);
//case '89Y'
    gamma.push_back(-1.3162791e7);
//case '90Zr'      
    gamma.push_back(0);
//case '91Zr'
    gamma.push_back(-2.49743e7);
//case '92Zr'     
    gamma.push_back(0);
//case '94Zr'     
    gamma.push_back(0);
//case '93Nb'
    gamma.push_back(6.5674e7);
//case '92Mo'      
    gamma.push_back(0);
//case '94Mo'      
    gamma.push_back(0);
//case '95Mo'
    gamma.push_back(-1.7514e7);
//case '96Mo'       
    gamma.push_back(0);
//case '97Mo'
    gamma.push_back(-1.7884e7);
//case '98Mo'      
    gamma.push_back(0);
//case '99Tc'
    gamma.push_back(6.0503e7);
//case '96Ru'       
    gamma.push_back(0);
//case '98Ru'      
    gamma.push_back(0);
//case '99Ru'
    gamma.push_back(-1.2286e7);
//case '100Ru'      
    gamma.push_back(0);
//case '101Ru'
    gamma.push_back(-1.3773e7);
//case '102Ru'      
    gamma.push_back(0);
//case '104Ru'        
    gamma.push_back(0);
//case '103Rh'
    gamma.push_back(-0.8468e7);
//case '102Pd'        
    gamma.push_back(0);
//case '104Pd'       
    gamma.push_back(0);
//case '105Pd'
    gamma.push_back(-1.2305e7);
//case '106Pd'       
    gamma.push_back(0);
//case '108Pd'      
    gamma.push_back(0);
//case '110Pd'        
    gamma.push_back(0);
//case '107Ag'
    gamma.push_back(-1.0889181e7);
//case '109Ag'
    gamma.push_back(-1.2518634e7);
//case '106Cd'       
    gamma.push_back(0);
//case '108Cd'        
    gamma.push_back(0);
//case '110Cd'       
    gamma.push_back(0);
//case '111Cd'
    gamma.push_back(-5.6983131e7);
//case '112Cd'      
    gamma.push_back(0);
//case '113Cd'
    gamma.push_back(-5.9609153e7);
//case '114Cd'        
    gamma.push_back(0);
//case '116Cd'       
    gamma.push_back(0);
//case '113In'
    gamma.push_back(5.8845e7);
//case '115In'
    gamma.push_back(5.8972e7);
//case '112Sn'       
    gamma.push_back(0);
//case '114Sn'      
    gamma.push_back(0);
//case '115Sn'
    gamma.push_back(-8.8013e7);
//case '116Sn'       
    gamma.push_back(0);
//case '117Sn'
    gamma.push_back(-9.58879e7);
//case '118Sn'      
    gamma.push_back(0);
//case '119Sn'
    gamma.push_back(-10.0317e7);
//case '120Sn'       
    gamma.push_back(0);
//case '122Sn'       
    gamma.push_back(0);
//case '124Sn'        
    gamma.push_back(0);
//case '121Sb'
    gamma.push_back(6.4435e7);
//case '123Sb'
    gamma.push_back(3.4892e7);
//case '120Te'      
    gamma.push_back(0);
//case '122Te'     
    gamma.push_back(0);
//case '123Te' 
    gamma.push_back(-7.059098e7);
//case '124Te'      
    gamma.push_back(0);
//case '125Te'
    gamma.push_back(-8.5108404e7);
//case '126Te'       
    gamma.push_back(0);
//case '128Te'       
    gamma.push_back(0);
//case '130Te'      
    gamma.push_back(0);
//case '127I'
    gamma.push_back(5.389573e7);
//case '124Xe'      
    gamma.push_back(0);
//case '126Xe'        
    gamma.push_back(0);
//case '128Xe'       
    gamma.push_back(0);
//case '129Xe'
    gamma.push_back(-7.452103e7);
//case '130Xe'      
    gamma.push_back(0);
//case '131Xe'
    gamma.push_back(2.209076e7);
//case '132Xe'      
    gamma.push_back(0);
//case '134Xe'      
    gamma.push_back(0);
//case '136Xe'      
    gamma.push_back(0);
//case '133Cs'
    gamma.push_back(3.5332539e7);
//case '130Ba'       
    gamma.push_back(0);
//case '132Ba'      
    gamma.push_back(0);
//case '134Ba'      
    gamma.push_back(0);
//case '135Ba'
    gamma.push_back(2.65750e7);
//case '136Ba'     
    gamma.push_back(0);
//case '137Ba'
    gamma.push_back(2.99295e7);
//case '138Ba'     
    gamma.push_back(0);
//case '138La'
    gamma.push_back(3.557239e7);
//case '139La'
    gamma.push_back(3.8083318e7);
//case '136Ce'      
    gamma.push_back(0);
//case '138Ce'     
    gamma.push_back(0);
//case '139Ce'
    gamma.push_back(2.906e7);
//case '140Ce'      
    gamma.push_back(0);
//case '142Ce'      
    gamma.push_back(0);
//case '141Pr'
    gamma.push_back(8.1907e7);
//case '142Nd'     
    gamma.push_back(0);
//case '143Nd'
    gamma.push_back(-1.457e7);
//case '144Nd'      
    gamma.push_back(0);
//case '145Nd'
    gamma.push_back(-0.898e7);
//case '146Nd'     
    gamma.push_back(0);
//case '148Nd'     
    gamma.push_back(0);
//case '150Nd'       
    gamma.push_back(0);
//case '147Pm'
    gamma.push_back(3.613e7);
//case '144Sm'     
    gamma.push_back(0);
//case '147Sm'
    gamma.push_back(-1.115e7);
//case '148Sm'        
    gamma.push_back(0);
//case '149Sm'
    gamma.push_back(-0.9192e7);
//case '150Sm'       
    gamma.push_back(0);
//case '152Sm'     
    gamma.push_back(0);
//case '154Sm'     
    gamma.push_back(0);
//case '151Eu'
    gamma.push_back(6.6510e7);
//case '153Eu'
    gamma.push_back(2.9369e7);
//case '152Gd'     
    gamma.push_back(0);
//case '154Gd'      
    gamma.push_back(0);
//case '155Gd'
    gamma.push_back(-0.82132e7);
//case '156Gd'      
    gamma.push_back(0);
//case '157Gd'
    gamma.push_back(-1.0769e7);
//case '158Gd'      
    gamma.push_back(0);
//case '160Gd'      
    gamma.push_back(0);
//case '159Tb'
    gamma.push_back(6.4306e7);
//case '156Dy'       
    gamma.push_back(0);
//case '158Dy'      
    gamma.push_back(0);
//case '160Dy'      
    gamma.push_back(0);
//case '161Dy'
    gamma.push_back(-0.9201e7);
//case '162Dy'      
    gamma.push_back(0);
//case '163Dy'
    gamma.push_back(1.289e7);
//case '164Dy'       
    gamma.push_back(0);
//case '165Ho'
    gamma.push_back(5.710e7);
//case '162Er'      
    gamma.push_back(0);
//case '164Er'       
    gamma.push_back(0);
//case '166Er'      
    gamma.push_back(0);
//case '167Er'
    gamma.push_back(-0.77157e7);
//case '168Er'      
    gamma.push_back(0);
//case '170Er'     
    gamma.push_back(0);
//case '169Tm'
    gamma.push_back(-2.218e7);
//case '168Yb'      
    gamma.push_back(0);
//case '170Yb'      
    gamma.push_back(0);
//case '171Yb'
    gamma.push_back(4.7288e7);
//case '172Yb'        
    gamma.push_back(0);
//case '173Yb'
    gamma.push_back(-1.3025e7);
//case '174Yb'        
    gamma.push_back(0);
//case '176Yb'        
    gamma.push_back(0);
//case '175Lu'
    gamma.push_back(3.0552e7);
//case '176Lu'
    gamma.push_back(2.1684e7);
//case '174Hf'       
    gamma.push_back(0);
//case '176Hf'        
    gamma.push_back(0);
//case '177Hf'
    gamma.push_back(1.086e7);
//case '178Hf'        
    gamma.push_back(0);
//case '179Hf'
    gamma.push_back(-0.6821e7);
//case '180Hf'       
    gamma.push_back(0);
//case '180Ta'       
    gamma.push_back(0);
//case '181Ta'
    gamma.push_back(3.2438e7);
//case '180W'      
    gamma.push_back(0);
//case '182W'       
    gamma.push_back(0);
//case '183W'
    gamma.push_back(1.1282403e7);
//case '184W'       
    gamma.push_back(0);
//case '186W'       
    gamma.push_back(0);
//case '185Re'
    gamma.push_back(6.1057e7);
//case '187Re'
    gamma.push_back(6.1682e7);
//case '184Os'        
    gamma.push_back(0);
//case '186Os'        
    gamma.push_back(0);
//case '187Os'
    gamma.push_back(0.6192895e7);
//case '188Os'       
    gamma.push_back(0);
//case '189Os'
    gamma.push_back(2.10713e7);
//case '190Os'      
    gamma.push_back(0);
//case '192Os'        
    gamma.push_back(0);
//case '191Ir'
    gamma.push_back(0.4812e7);
//case '193Ir'
    gamma.push_back(0.5227e7);
//case '190Pt'   
    gamma.push_back(0);
//case '192Pt'       
    gamma.push_back(0);
//case '194Pt'       
    gamma.push_back(0);
//case '195Pt'
    gamma.push_back(5.8385e7);
//case '196Pt'      
    gamma.push_back(0);
//case '198Pt'     
    gamma.push_back(0);
//case '197Au'
    gamma.push_back(0.473060e7);
//case '196Hg'       
    gamma.push_back(0);
//case '198Hg'      
    gamma.push_back(0);
//case '199Hg'
    gamma.push_back(4.8457916e7);
//case '200Hg'     
    gamma.push_back(0);
//case '201Hg'
    gamma.push_back(-1.788769e7);
//case '202Hg'     
    gamma.push_back(0);
//case '204Hg'       
    gamma.push_back(0);
//case '203Tl'
    gamma.push_back(15.5393338e7);
//case '205Tl'
    gamma.push_back(15.6921808e7);
//case '204Pb'       
    gamma.push_back(0);
//case '206Pb'     
    gamma.push_back(0);
//case '207Pb'
    gamma.push_back(5.58046e7);
//case '208Pb'       
    gamma.push_back(0);
//case '209Bi'
    gamma.push_back(4.3750e7);
//case '209Po'
    gamma.push_back(7.4e7);
//case '227Ac'
    gamma.push_back(3.5e7);
//case '229Th'
    gamma.push_back(0.40e7);
//case '231Pa'
    gamma.push_back(3.21e7);
//case '235U'
    gamma.push_back(-0.4926e7);
//case '237Np'
    gamma.push_back(3.1e7);
//case '239Pu'
    gamma.push_back(0.972e7);
//case '241Am' 
    gamma.push_back(1.54e7);
//case '243Am' 
    gamma.push_back(1.54e7);
//case '247Cm'
    gamma.push_back(0.20e7);
}

double get_gyromagnetic_ratio(std::string isotope, int index, bool rad)
{
    if (index < 0)
    {
        if (isotope == "E")
        {
            index = 0;
        }
        if (isotope == "N")
        {
            index = 1;
        }
        if (isotope == "M")
        {
            index = 2;
        }
        if (isotope == "P")
        {
            index = 3;
        }
        if (isotope == "1H")
        {
            index = 4;
        }
        if (isotope == "2H")
        {
            index = 5;
        }
        if (isotope == "3H")
        {
            index = 6;
        }
        if (isotope == "3He")
        {
            index = 7;
        }
        if (isotope == "4He")
        {
            index = 8;
        }
        if (isotope == "6Li")
        {
            index = 9;
        }
        if (isotope == "7Li")
        {
            index = 10;
        }
        if (isotope == "9Be")
        {
            index = 11;
        }
        if (isotope == "10B")
        {
            index = 12;
        }
        if (isotope == "11B")
        {
            index = 13;
        }
        if (isotope == "12C")
        {
            index = 14;
        }
        if (isotope == "13C")
        {
            index = 15;
        }
        if (isotope == "14N")
        {
            index = 16;
        }
        if (isotope == "15N")
        {
            index = 17;
        }
        if (isotope == "16O")
        {
            index = 18;
        }
        if (isotope == "17O")
        {
            index = 19;
        }
        if (isotope == "18O")
        {
            index = 20;
        }
        if (isotope == "19F")
        {
            index = 21;
        }
        if (isotope == "20Ne")
        {
            index = 22;
        }
        if (isotope == "21Ne")
        {
            index = 23;
        }
        if (isotope == "22Ne")
        {
            index = 24;
        }
        if (isotope == "23Na")
        {
            index = 25;
        }
        if (isotope == "24Mg")
        {
            index = 26;
        }
        if (isotope == "25Mg")
        {
            index = 27;
        }
        if (isotope == "26Mg")
        {
            index = 28;
        }
        if (isotope == "27Al")
        {
            index = 29;
        }
        if (isotope == "28Si")
        {
            index = 30;
        }
        if (isotope == "29Si")
        {
            index = 31;
        }
        if (isotope == "30Si")
        {
            index = 32;
        }
        if (isotope == "31P")
        {
            index = 33;
        }
        if (isotope == "32S")
        {
            index = 34;
        }
        if (isotope == "33S")
        {
            index = 35;
        }
        if (isotope == "34S")
        {
            index = 36;
        }
        if (isotope == "36S")
        {
            index = 37;
        }
        if (isotope == "35Cl")
        {
            index = 38;
        }
        if (isotope == "37Cl")
        {
            index = 39;
        }
        if (isotope == "36Ar")
        {
            index = 40;
        }
        if (isotope == "38Ar")
        {
            index = 41;
        }
        if (isotope == "39Ar")
        {
            index = 42;
        }
        if (isotope == "40Ar")
        {
            index = 43;
        }
        if (isotope == "39K")
        {
            index = 44;
        }
        if (isotope == "40K")
        {
            index = 45;
        }
        if (isotope == "41K")
        {
            index = 46;
        }
        if (isotope == "40Ca")
        {
            index = 47;
        }
        if (isotope == "41Ca")
        {
            index = 48;
        }
        if (isotope == "42Ca")
        {
            index = 49;
        }
        if (isotope == "43Ca")
        {
            index = 50;
        }
        if (isotope == "44Ca")
        {
            index = 51;
        }
        if (isotope == "45Sc")
        {
            index = 52;
        }
        if (isotope == "46Ti")
        {
            index = 53;
        }
        if (isotope == "47Ti")
        {
            index = 54;
        }
        if (isotope == "48Ti")
        {
            index = 55;
        }
        if (isotope == "49Ti")
        {
            index = 56;
        }
        if (isotope == "50Ti")
        {
            index = 57;
        }
        if (isotope == "50V")
        {
            index = 58;
        }
        if (isotope == "51V")
        {
            index = 59;
        }
        if (isotope == "52Cr")
        {
            index = 60;
        }
        if (isotope == "53Cr")
        {
            index = 61;
        }
        if (isotope == "54Cr")
        {
            index = 62;
        }
        if (isotope == "55Mn")
        {
            index = 63;
        }
        if (isotope == "54Fe")
        {
            index = 64;
        }
        if (isotope == "56Fe")
        {
            index = 65;
        }
        if (isotope == "57Fe")
        {
            index = 66;
        }
        if (isotope == "58Fe")
        {
            index = 67;
        }
        if (isotope == "59Co")
        {
            index = 68;
        }
        if (isotope == "60Ni")
        {
            index = 69;
        }
        if (isotope == "61Ni")
        {
            index = 70;
        }
        if (isotope == "62Ni")
        {
            index = 71;
        }
        if (isotope == "64Ni")
        {
            index = 72;
        }
        if (isotope == "63Cu")
        {
            index = 73;
        }
        if (isotope == "65Cu")
        {
            index = 74;
        }
        if (isotope == "64Zn")
        {
            index = 75;
        }
        if (isotope == "66Zn")
        {
            index = 76;
        }
        if (isotope == "67Zn")
        {
            index = 77;
        }
        if (isotope == "68Zn")
        {
            index = 78;
        }
        if (isotope == "69Ga")
        {
            index = 79;
        }
        if (isotope == "71Ga")
        {
            index = 80;
        }
        if (isotope == "70Ge")
        {
            index = 81;
        }
        if (isotope == "72Ge")
        {
            index = 82;
        }
        if (isotope == "73Ge")
        {
            index = 83;
        }
        if (isotope == "74Ge")
        {
            index = 84;
        }
        if (isotope == "75As")
        {
            index = 85;
        }
        if (isotope == "74Se")
        {
            index = 86;
        }
        if (isotope == "76Se")
        {
            index = 87;
        }
        if (isotope == "77Se")
        {
            index = 88;
        }
        if (isotope == "78Se")
        {
            index = 89;
        }
        if (isotope == "79Br")
        {
            index = 90;
        }
        if (isotope == "81Br")
        {
            index = 91;
        }
        if (isotope == "80Kr")
        {
            index = 92;
        }
        if (isotope == "82Kr")
        {
            index = 93;
        }
        if (isotope == "83Kr")
        {
            index = 94;
        }
        if (isotope == "84Kr")
        {
            index = 95;
        }
        if (isotope == "86Kr")
        {
            index = 96;
        }
        if (isotope == "85Rb")
        {
            index = 97;
        }
        if (isotope == "87Rb")
        {
            index = 98;
        }
        if (isotope == "84Sr")
        {
            index = 99;
        }
        if (isotope == "86Sr")
        {
            index = 100;
        }
        if (isotope == "87Sr")
        {
            index = 101;
        }
        if (isotope == "88Sr")
        {
            index = 102;
        }
        if (isotope == "89Y")
        {
            index = 103;
        }
        if (isotope == "90Zr")
        {
            index = 104;
        }
        if (isotope == "91Zr")
        {
            index = 105;
        }
        if (isotope == "92Zr")
        {
            index = 106;
        }
        if (isotope == "94Zr")
        {
            index = 107;
        }
        if (isotope == "93Nb")
        {
            index = 108;
        }
        if (isotope == "92Mo")
        {
            index = 109;
        }
        if (isotope == "94Mo")
        {
            index = 110;
        }
        if (isotope == "95Mo")
        {
            index = 111;
        }
        if (isotope == "96Mo")
        {
            index = 112;
        }
        if (isotope == "97Mo")
        {
            index = 113;
        }
        if (isotope == "98Mo")
        {
            index = 114;
        }
        if (isotope == "99Tc")
        {
            index = 115;
        }
        if (isotope == "96Ru")
        {
            index = 116;
        }
        if (isotope == "98Ru")
        {
            index = 117;
        }
        if (isotope == "99Ru")
        {
            index = 118;
        }
        if (isotope == "100Ru")
        {
            index = 119;
        }
        if (isotope == "101Ru")
        {
            index = 120;
        }
        if (isotope == "102Ru")
        {
            index = 121;
        }
        if (isotope == "104Ru")
        {
            index = 122;
        }
        if (isotope == "103Rh")
        {
            index = 123;
        }
        if (isotope == "102Pd")
        {
            index = 124;
        }
        if (isotope == "104Pd")
        {
            index = 125;
        }
        if (isotope == "105Pd")
        {
            index = 126;
        }
        if (isotope == "106Pd")
        {
            index = 127;
        }
        if (isotope == "108Pd")
        {
            index = 128;
        }
        if (isotope == "110Pd")
        {
            index = 129;
        }
        if (isotope == "107Ag")
        {
            index = 130;
        }
        if (isotope == "109Ag")
        {
            index = 131;
        }
        if (isotope == "106Cd")
        {
            index = 132;
        }
        if (isotope == "108Cd")
        {
            index = 133;
        }
        if (isotope == "110Cd")
        {
            index = 134;
        }
        if (isotope == "111Cd")
        {
            index = 135;
        }
        if (isotope == "112Cd")
        {
            index = 136;
        }
        if (isotope == "113Cd")
        {
            index = 137;
        }
        if (isotope == "114Cd")
        {
            index = 138;
        }
        if (isotope == "116Cd")
        {
            index = 139;
        }
        if (isotope == "113In")
        {
            index = 140;
        }
        if (isotope == "115In")
        {
            index = 141;
        }
        if (isotope == "112Sn")
        {
            index = 142;
        }
        if (isotope == "114Sn")
        {
            index = 143;
        }
        if (isotope == "115Sn")
        {
            index = 144;
        }
        if (isotope == "116Sn")
        {
            index = 145;
        }
        if (isotope == "117Sn")
        {
            index = 146;
        }
        if (isotope == "118Sn")
        {
            index = 147;
        }
        if (isotope == "119Sn")
        {
            index = 148;
        }
        if (isotope == "120Sn")
        {
            index = 149;
        }
        if (isotope == "122Sn")
        {
            index = 150;
        }
        if (isotope == "124Sn")
        {
            index = 151;
        }
        if (isotope == "121Sb")
        {
            index = 152;
        }
        if (isotope == "123Sb")
        {
            index = 153;
        }
        if (isotope == "120Te")
        {
            index = 154;
        }
        if (isotope == "122Te")
        {
            index = 155;
        }
        if (isotope == "123Te")
        {
            index = 156;
        }
        if (isotope == "124Te")
        {
            index = 157;
        }
        if (isotope == "125Te")
        {
            index = 158;
        }
        if (isotope == "126Te")
        {
            index = 159;
        }
        if (isotope == "128Te")
        {
            index = 160;
        }
        if (isotope == "130Te")
        {
            index = 161;
        }
        if (isotope == "127I")
        {
            index = 162;
        }
        if (isotope == "124Xe")
        {
            index = 163;
        }
        if (isotope == "126Xe")
        {
            index = 164;
        }
        if (isotope == "128Xe")
        {
            index = 165;
        }
        if (isotope == "129Xe")
        {
            index = 166;
        }
        if (isotope == "130Xe")
        {
            index = 167;
        }
        if (isotope == "131Xe")
        {
            index = 168;
        }
        if (isotope == "132Xe")
        {
            index = 169;
        }
        if (isotope == "134Xe")
        {
            index = 170;
        }
        if (isotope == "136Xe")
        {
            index = 171;
        }
        if (isotope == "133Cs")
        {
            index = 172;
        }
        if (isotope == "130Ba")
        {
            index = 173;
        }
        if (isotope == "132Ba")
        {
            index = 174;
        }
        if (isotope == "134Ba")
        {
            index = 175;
        }
        if (isotope == "135Ba")
        {
            index = 176;
        }
        if (isotope == "136Ba")
        {
            index = 177;
        }
        if (isotope == "137Ba")
        {
            index = 178;
        }
        if (isotope == "138Ba")
        {
            index = 179;
        }
        if (isotope == "138La")
        {
            index = 180;
        }
        if (isotope == "139La")
        {
            index = 181;
        }
        if (isotope == "136Ce")
        {
            index = 182;
        }
        if (isotope == "138Ce")
        {
            index = 183;
        }
        if (isotope == "139Ce")
        {
            index = 184;
        }
        if (isotope == "140Ce")
        {
            index = 185;
        }
        if (isotope == "142Ce")
        {
            index = 186;
        }
        if (isotope == "141Pr")
        {
            index = 187;
        }
        if (isotope == "142Nd")
        {
            index = 188;
        }
        if (isotope == "143Nd")
        {
            index = 189;
        }
        if (isotope == "144Nd")
        {
            index = 190;
        }
        if (isotope == "145Nd")
        {
            index = 191;
        }
        if (isotope == "146Nd")
        {
            index = 192;
        }
        if (isotope == "148Nd")
        {
            index = 193;
        }
        if (isotope == "150Nd")
        {
            index = 194;
        }
        if (isotope == "147Pm")
        {
            index = 195;
        }
        if (isotope == "144Sm")
        {
            index = 196;
        }
        if (isotope == "147Sm")
        {
            index = 197;
        }
        if (isotope == "148Sm")
        {
            index = 198;
        }
        if (isotope == "149Sm")
        {
            index = 199;
        }
        if (isotope == "150Sm")
        {
            index = 200;
        }
        if (isotope == "152Sm")
        {
            index = 201;
        }
        if (isotope == "154Sm")
        {
            index = 202;
        }
        if (isotope == "151Eu")
        {
            index = 203;
        }
        if (isotope == "153Eu")
        {
            index = 204;
        }
        if (isotope == "152Gd")
        {
            index = 205;
        }
        if (isotope == "154Gd")
        {
            index = 206;
        }
        if (isotope == "155Gd")
        {
            index = 207;
        }
        if (isotope == "156Gd")
        {
            index = 208;
        }
        if (isotope == "157Gd")
        {
            index = 209;
        }
        if (isotope == "158Gd")
        {
            index = 210;
        }
        if (isotope == "160Gd")
        {
            index = 211;
        }
        if (isotope == "159Tb")
        {
            index = 212;
        }
        if (isotope == "156Dy")
        {
            index = 213;
        }
        if (isotope == "158Dy")
        {
            index = 214;
        }
        if (isotope == "160Dy")
        {
            index = 215;
        }
        if (isotope == "161Dy")
        {
            index = 216;
        }
        if (isotope == "162Dy")
        {
            index = 217;
        }
        if (isotope == "163Dy")
        {
            index = 218;
        }
        if (isotope == "164Dy")
        {
            index = 219;
        }
        if (isotope == "165Ho")
        {
            index = 220;
        }
        if (isotope == "162Er")
        {
            index = 221;
        }
        if (isotope == "164Er")
        {
            index = 222;
        }
        if (isotope == "166Er")
        {
            index = 223;
        }
        if (isotope == "167Er")
        {
            index = 224;
        }
        if (isotope == "168Er")
        {
            index = 225;
        }
        if (isotope == "170Er")
        {
            index = 226;
        }
        if (isotope == "169Tm")
        {
            index = 227;
        }
        if (isotope == "168Yb")
        {
            index = 228;
        }
        if (isotope == "170Yb")
        {
            index = 229;
        }
        if (isotope == "171Yb")
        {
            index = 230;
        }
        if (isotope == "172Yb")
        {
            index = 231;
        }
        if (isotope == "173Yb")
        {
            index = 232;
        }
        if (isotope == "174Yb")
        {
            index = 233;
        }
        if (isotope == "176Yb")
        {
            index = 234;
        }
        if (isotope == "175Lu")
        {
            index = 235;
        }
        if (isotope == "176Lu")
        {
            index = 236;
        }
        if (isotope == "174Hf")
        {
            index = 237;
        }
        if (isotope == "176Hf")
        {
            index = 238;
        }
        if (isotope == "177Hf")
        {
            index = 239;
        }
        if (isotope == "178Hf")
        {
            index = 240;
        }
        if (isotope == "179Hf")
        {
            index = 241;
        }
        if (isotope == "180Hf")
        {
            index = 242;
        }
        if (isotope == "180Ta")
        {
            index = 243;
        }
        if (isotope == "181Ta")
        {
            index = 244;
        }
        if (isotope == "180W")
        {
            index = 245;
        }
        if (isotope == "182W")
        {
            index = 246;
        }
        if (isotope == "183W")
        {
            index = 247;
        }
        if (isotope == "184W")
        {
            index = 248;
        }
        if (isotope == "186W")
        {
            index = 249;
        }
        if (isotope == "185Re")
        {
            index = 250;
        }
        if (isotope == "187Re")
        {
            index = 251;
        }
        if (isotope == "184Os")
        {
            index = 252;
        }
        if (isotope == "186Os")
        {
            index = 253;
        }
        if (isotope == "187Os")
        {
            index = 254;
        }
        if (isotope == "188Os")
        {
            index = 255;
        }
        if (isotope == "189Os")
        {
            index = 256;
        }
        if (isotope == "190Os")
        {
            index = 257;
        }
        if (isotope == "192Os")
        {
            index = 258;
        }
        if (isotope == "191Ir")
        {
            index = 259;
        }
        if (isotope == "193Ir")
        {
            index = 260;
        }
        if (isotope == "190Pt")
        {
            index = 261;
        }
        if (isotope == "192Pt")
        {
            index = 262;
        }
        if (isotope == "194Pt")
        {
            index = 263;
        }
        if (isotope == "195Pt")
        {
            index = 264;
        }
        if (isotope == "196Pt")
        {
            index = 265;
        }
        if (isotope == "198Pt")
        {
            index = 266;
        }
        if (isotope == "197Au")
        {
            index = 267;
        }
        if (isotope == "196Hg")
        {
            index = 268;
        }
        if (isotope == "198Hg")
        {
            index = 269;
        }
        if (isotope == "199Hg")
        {
            index = 270;
        }
        if (isotope == "200Hg")
        {
            index = 271;
        }
        if (isotope == "201Hg")
        {
            index = 272;
        }
        if (isotope == "202Hg")
        {
            index = 273;
        }
        if (isotope == "204Hg")
        {
            index = 274;
        }
        if (isotope == "203Tl")
        {
            index = 275;
        }
        if (isotope == "205Tl")
        {
            index = 276;
        }
        if (isotope == "204Pb")
        {
            index = 277;
        }
        if (isotope == "206Pb")
        {
            index = 278;
        }
        if (isotope == "207Pb")
        {
            index = 279;
        }
        if (isotope == "208Pb")
        {
            index = 280;
        }
        if (isotope == "209Bi")
        {
            index = 281;
        }
        if (isotope == "209Po")
        {
            index = 282;
        }
        if (isotope == "227Ac")
        {
            index = 283;
        }
        if (isotope == "229Th")
        {
            index = 284;
        }
        if (isotope == "231Pa")
        {
            index = 285;
        }
        if (isotope == "235U")
        {
            index = 286;
        }
        if (isotope == "237Np")
        {
            index = 287;
        }
        if (isotope == "239Pu")
        {
            index = 288;
        }
        if (isotope == "241Am")
        {
            index = 289;
        }
        if (isotope == "243Am")
        {
            index = 290;
        }
        if (isotope == "247Cm")
        {
            index = 291;
        }
        if (index < 0)
        {
            index = 8;
        }

    }
    if (index < 0)
    {
        index = 0;
    }
    if (rad == true)
    {
        return gamma[index];
    }
    else
    {
        return (gamma[index] / (2 * PI));
    }
}

std::vector<std::vector<double>> MatrixMultiplication(std::vector<std::vector<double>> left, std::vector<std::vector<double>> right)
{
    std::vector<std::vector<double>> resultmat(left.size(), std::vector<double>(right[0].size(), 0.0));
    for (size_t i = 0; i < left.size(); i++)
    {
        for (size_t j = 0; j < right[0].size(); j++)
        {
            for (size_t a = 0; a < left[0].size(); a++)
            {
                resultmat[i][j] += left[i][a] * right[a][j];
            }
        }
    }
    return resultmat;
}

std::vector<std::vector<double>> transpose(std::vector<std::vector<double>> matrix)
{
    std::vector<std::vector<double>> return_matrix(matrix[0].size(), std::vector<double>(matrix.size(), 0.0));
    for (size_t i = 0; i < matrix.size(); i++)
    {
        for (size_t j = 0; j < matrix[i].size(); j++)
        {
            return_matrix[j][i] = matrix[i][j];
        }
    }
    return return_matrix;
}

long double determinant(std::vector<std::vector<double>> matrix, bool first_call)
{
    size_t dimension = matrix.size();
    long double k = 0.0;
    if (first_call)
    {
        double max = -INFINITY;
        for (const auto& row : matrix)
        {
            for (const auto& element : row)
            {
                if (abs(element) > max)
                {
                    max = abs(element);
                }
            }
        }
        k = 1.0 / max;
        for (auto& row : matrix)
        {
            for (auto& element : row)
            {
                element *= k;
            }
        }
    }
    long double detA = 0;
    for (size_t j = 0; j < dimension; j++)
    {
        std::vector<std::vector<double>> Untermatrix;
        for (size_t i = 1; i < dimension; i++)
        {
            Untermatrix.emplace_back(std::vector<double>());
            for (size_t j1 = 0; j1 < dimension; j1++)
            {
                if (j1 != j)
                {
                    Untermatrix[Untermatrix.size() - 1].emplace_back(matrix[i][j1]);
                }
            }
        }
        if (Untermatrix.size() != 1)
        {
            detA += pow(-1.0, j) * matrix[0][j] * determinant(Untermatrix, false);
        }
        else
        {
            detA += pow(-1.0, j) * matrix[0][j] * Untermatrix[0][0];
        }
    }
    if (first_call)
    {
        k = pow(k, dimension);
        if (abs(detA) < 1e-12)
        {
            detA = 0.0;
        }
        else
        {
            detA /= k;
        }
    }
    return detA;
}

std::vector<std::vector<double>> eigenvectors(std::vector<std::vector<double>> matrix, std::vector<double>& eigenvalues, std::string& warning)
{
    bool contin = false;
    for (const auto& row : matrix)
    {
        for (const auto& element : row)
        {
            if (element != 0.0)
            {
                contin = true;
            }
        }
    }
    if (!contin)
    {
        eigenvalues = std::vector<double>(3, 0.0);
        return std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0));
    }
    eigenvalues.clear();
    std::vector<std::vector<long double>> eigenvector(3, std::vector<long double>(3, 0.0));
    long double a = (long double)matrix[0][0] + (long double)matrix[1][1] + (long double)matrix[2][2];
    long double b = (long double)matrix[0][1] * (long double)matrix[1][0] + (long double)matrix[1][2] * (long double)matrix[2][1] + (long double)matrix[0][2] * (long double)matrix[2][0];
    b -= ((long double)matrix[0][0] * (long double)matrix[1][1] + (long double)matrix[1][1] * (long double)matrix[2][2] + (long double)matrix[2][2] * (long double)matrix[0][0]);
    long double c = (long double)determinant(matrix, true);
    a *= -1.0;
    b *= -1.0;
    c *= -1.0;
    long double p = b - (pow(a, 2.0) / 3.0);
    long double q = ((2.0 * pow(a, 3.0)) / 27.0) - ((a * b) / 3.0) + c;
    long double diskrim = pow(q / 2.0, 2.0) + pow(p / 3.0, 3.0);
    long double z1, z2, z3;
    if (diskrim > 0.0)
    {
        return { {0.0} };
    }
    else if (diskrim == 0.0)
    {
        z1 = 2.0 * pow(-q / 2.0, 1.0 / 3.0);
        z2 = -pow(-q / 2.0, 1.0 / 3.0);
        z3 = z2;
        AfxMessageBox(_T("untested case discriminant = 0"), MB_ICONINFORMATION);
    }
    else if (diskrim < 0.0)
    {
        long double r = sqrt(-pow(p, 3.0) / 27.0);
        long double phi = acos(-q / (2.0 * r));
        z1 = 2.0 * pow(r, 1.0 / 3.0) * cos((phi + 2.0 * 1.0 * PI) / 3.0);
        z2 = 2.0 * pow(r, 1.0 / 3.0) * cos((phi + 2.0 * 2.0 * PI) / 3.0);
        z3 = 2.0 * pow(r, 1.0 / 3.0) * cos((phi + 2.0 * 3.0 * PI) / 3.0);
    }
    long double eigenvalue1 = z1 - a / 3.0;
    long double eigenvalue2 = z2 - a / 3.0;
    long double eigenvalue3 = z3 - a / 3.0;
    if (abs(eigenvalue1) < 1e-12)
    {
        eigenvalue1 = 0.0;
    }
    if (abs(eigenvalue2) < 1e-12)
    {
        eigenvalue2 = 0.0;
    }
    if (abs(eigenvalue3) < 1e-12)
    {
        eigenvalue3 = 0.0;
    }
    std::vector<std::vector<std::vector<long double>>> A(3, std::vector<std::vector<long double>>(3, std::vector<long double>(3,0.0)));
    for (size_t mat = 0; mat < 3; mat++)
    {
        for (size_t row = 0; row < 3; row++)
        {
            for (size_t element = 0; element < 3; element++)
            {
                A[mat][row][element] = (long double)matrix[row][element];
            }
        }
    }
    A[0][0][0] -= eigenvalue1;
    A[0][1][1] -= eigenvalue1;
    A[0][2][2] -= eigenvalue1;
    A[1][0][0] -= eigenvalue2;
    A[1][1][1] -= eigenvalue2;
    A[1][2][2] -= eigenvalue2;
    A[2][0][0] -= eigenvalue3;
    A[2][1][1] -= eigenvalue3;
    A[2][2][2] -= eigenvalue3;
    eigenvalues = { (double)eigenvalue1, (double)eigenvalue2, (double)eigenvalue3 };
    for (size_t mat = 0; mat < 3; mat++)
    {
        for (size_t i = 0; i < 3; i++)
        {
            if (abs(A[mat][i][i]) < 1e-8)
            {
                A[mat][i][i] = 0.0;
            }
        }
    }
    std::vector<std::vector<long double>> tmp_mat;
    for (size_t mat = 0; mat < 3; mat++)
    {
        tmp_mat.clear();
        std::vector<bool> found_piv(3, false);
        for (size_t i = 0; i < 3; i++)
        {
            if (A[mat][i][0] != 0.0)
            {
                found_piv[i] = true;
                tmp_mat.push_back(A[mat][i]);
            }
        }
        for (size_t i = 0; i < 3; i++)
        {
            if (!found_piv[i])
            {
                tmp_mat.push_back(A[mat][i]);
            }
        }
        A[mat].clear();
        A[mat] = tmp_mat;
        tmp_mat.clear();
        if (A[mat][0][0] != 0.0)
        {
            long double factor = A[mat][0][0];
            for (size_t i = 0; i < 3; i++)
            {
                A[mat][0][i] /= factor;
            }
            for (size_t i = 1; i < 3; i++)
            {
                factor = A[mat][i][0];
                for (size_t j = 0; j < 3; j++)
                {
                    A[mat][i][j] -= A[mat][0][j] * factor;
                }
            }
        }
        for (auto& mat : A)
        {
            for (auto& row : mat)
            {
                for (auto& element : row)
                {
                    if (abs(element) < 1e-12)
                    {
                        element = 0.0;
                    }
                }
            }
        }
        found_piv.clear();
        if (A[mat][0][0] != 0.0)
        {
            tmp_mat.push_back(A[mat][0]);
            if (A[mat][1][1] == 0.0 && A[mat][2][1] != 0.0)
            {
                tmp_mat.push_back(A[mat][2]);
                tmp_mat.push_back(A[mat][1]);
            }
            else
            {
                tmp_mat.push_back(A[mat][1]);
                tmp_mat.push_back(A[mat][2]);
            }
            A[mat].clear();
            A[mat] = tmp_mat;
        }
        else
        {
            for (size_t i = 0; i < 3; i++)
            {
                if (A[mat][i][1] != 0.0)
                {
                    tmp_mat.push_back(A[mat][i]);
                    for (size_t j = 0; j < 3; j++)
                    {
                        if (j != i)
                        {
                            tmp_mat.push_back(A[mat][j]);
                        }
                    }
                }
            }
            if (tmp_mat.size() != 0)
            {
                A[mat][1] = tmp_mat[0];
                A[mat][0] = tmp_mat[1];
                A[mat][2] = tmp_mat[2];
            }
        }
        tmp_mat.clear();
        if (A[mat][1][1] != 0.0)
        {
            long double factor = A[mat][1][1];
            for (size_t i = 0; i < 3; i++)
            {
                A[mat][1][i] /= factor;
            }
            for (size_t i = 0; i < 3; i++)
            {
                if (i != 1 && A[mat][i][1] != 0.0 && A[mat][i][0] == 0.0)
                {
                    factor = A[mat][i][1];
                    for (size_t j = 0; j < 3; j++)
                    {
                        A[mat][i][j] -= factor * A[mat][1][j];
                    }
                }
            }
        }
        for (auto& mat : A)
        {
            for (auto& row : mat)
            {
                for (auto& element : row)
                {
                    if (abs(element) < 1e-12)
                    {
                        element = 0.0;
                    }
                }
            }
        }
        for (size_t i = 0; i < 3; i++)
        {
            if (A[mat][i][0] == 0.0 && A[mat][i][1] == 0.0 && A[mat][i][2] != 0.0)
            {
                for (size_t j = 0; j < 3; j++)
                {
                    if (j != i)
                    {
                        tmp_mat.push_back(A[mat][j]);
                    }
                }
                tmp_mat.push_back(A[mat][i]);
            }
        }
        if (tmp_mat.size() != 0)
        {
            A[mat].clear();
            A[mat] = tmp_mat;
        }
        if (abs(A[mat][2][2]) < 1e-4)
        {
            eigenvector[mat][2] = 1.0;
            if (abs(A[mat][2][2]) > 1e-8)
            {
                warning += "a relatively large coefficient was detected during the computation of eigenvector " + std::to_string(mat+1) + " (1e-8 < val < 1e-4)\n";
            }
        }
        if (A[mat][1][1] == 0.0)
        {
            eigenvector[mat][1] = 1.0;
        }
        else
        {
            eigenvector[mat][1] = - A[mat][1][2] * eigenvector[mat][2] / A[mat][1][1];
        }
        if (A[mat][0][0] == 0.0)
        {
            eigenvector[mat][0] = 1.0;
        }
        else
        {
            eigenvector[mat][0] = -(A[mat][0][1] * eigenvector[mat][1] + A[mat][0][2] * eigenvector[mat][2]) / A[mat][0][0];
        }
        for (size_t i = 0; i < 3; i++)
        {
            long double length = pow(eigenvector[mat][0], 2.0) + pow(eigenvector[mat][1], 2.0) + pow(eigenvector[mat][2], 2.0);
            length = sqrt(length);
            for (size_t j = 0; j < 3; j++)
            {
                eigenvector[mat][j] /= length;
            }
        }
    }
    std::vector<std::vector<double>> return_eigenvector(3, std::vector<double>(3, 0.0));
    for (size_t row = 0; row < 3; row++)
    {
        for (size_t element = 0; element < 3; element++)
        {
            return_eigenvector[row][element] = (double)eigenvector[element][row];
        }
    }
    return return_eigenvector;
}

std::vector<std::vector<double>> inverse_Matrix(std::vector<std::vector<double>> original_matrix)
{
    std::vector<std::vector<double>> matrix(3, std::vector<double>(6, 0.0));
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            matrix[i][j] = original_matrix[i][j];
        }
        matrix[i][3 + i] = 1.0;
    }
    for (size_t col = 0; col < 3; col++)
    {
        int main_row = -1;
        for (size_t row = col; row < 3; row++)
        {
            if (matrix[row][col] != 0.0)
            {
                main_row = static_cast<int>(row);
                break;
            }
        }
        if (main_row == -1) { return { {0.0} }; }
        if (main_row != static_cast<int>(col))
        {
            std::swap(matrix[col], matrix[main_row]);
        }
        double value = matrix[col][col];
        for (size_t j = 0; j < 6; j++)
        {
            matrix[col][j] /= value;
        }
        for (size_t row = 0; row < 3; row++)
        {
            if (row != col)
            {
                value = matrix[row][col];
                for (size_t j = 0; j < 6; j++)
                {
                    matrix[row][j] -= value * matrix[col][j];
                }
            }
        }
    }
    std::vector<std::vector<double>> result(3, std::vector<double>(3));
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            result[i][j] = matrix[i][j + 3];
        }
    }
    return result;
}