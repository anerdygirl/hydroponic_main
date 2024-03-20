#ifndef __FUZZY_H__
#define __FUZZY_H__
#include <Fuzzy.h>

// for defuzzification
typedef struct{
  int i_wt;
  int i_nt;
  int i_d;
} fuzzyoutputs;

Fuzzy* fuzzy = new Fuzzy();

// membership functions for: pH lvl, EC, Water lvl, water tank, nutrient tank and nutrient tank drain
FuzzySet* pH_low = new FuzzySet(0, 0, 5.5, 5.8);
FuzzySet* pH_within = new FuzzySet(5.5, 5.8, 6.2, 6.5);
FuzzySet* pH_high = new FuzzySet(6.2, 6.5, 6.7, 6.7);

FuzzySet* EC_low = new FuzzySet(0, 0, 0.8, 0.9);
FuzzySet* EC_within = new FuzzySet(0.8, 0.9, 1.1, 1.2);
FuzzySet* EC_high = new FuzzySet(1.1, 1.2, 1.4, 1.4);

FuzzySet* WL_low = new FuzzySet(0, 0, 50, 55);
FuzzySet* WL_within = new FuzzySet(50, 55, 65, 70);
FuzzySet* WL_full = new FuzzySet(65, 70, 80, 80);

// membership fcts left empty because the parameters are unknown.. for now
FuzzySet* WT_on = new FuzzySet();
FuzzySet* WT_off = new FuzzySet();

FuzzySet* NT_on = new FuzzySet();
FuzzySet* NT_off = new FuzzySet();

FuzzySet* D_on = new FuzzySet();
FuzzySet* D_off = new FuzzySet();


void setup_fuzzy() {
  // inputs, outputs and rule base
  // pH input set
  FuzzyInput* pH = new FuzzyInput(1);
  pH->addFuzzySet(pH_low);
  pH->addFuzzySet(pH_within);
  pH->addFuzzySet(pH_high);
  fuzzy->addFuzzyInput(pH);

  // EC input set
  FuzzyInput* EC = new FuzzyInput(2);
  EC->addFuzzySet(EC_low);
  EC->addFuzzySet(EC_within);
  EC->addFuzzySet(EC_high);
  fuzzy->addFuzzyInput(EC);

  // Water level input set. unit is liters
  FuzzyInput* WL = new FuzzyInput(3);
  WL->addFuzzySet(WL_low);
  WL->addFuzzySet(WL_within);
  WL->addFuzzySet(WL_full);
  fuzzy->addFuzzyInput(WL);

  // water tank output set
  FuzzyOutput* WT = new FuzzyOutput(1);
  WT->addFuzzySet(WT_on);
  WT->addFuzzySet(WT_off);
  fuzzy->addFuzzyOutput(WT);

  // nutrient tank output set
  FuzzyOutput* NT = new FuzzyOutput(2);
  NT->addFuzzySet(NT_on);
  NT->addFuzzySet(NT_off);
  fuzzy->addFuzzyOutput(NT);

  // nutrient tank Drain output set
  FuzzyOutput* D = new FuzzyOutput(3);
  D->addFuzzySet(D_on);
  D->addFuzzySet(D_off);
  fuzzy->addFuzzyOutput(D);

  /* rules */
  // Consequents (then statements)
  FuzzyRuleConsequent* then_WT_off_NT_off_D_off = new FuzzyRuleConsequent();
  then_WT_on_NT_on_D_off->addOutput(WT_off);
  then_WT_on_NT_on_D_off->addOutput(NT_off);
  then_WT_on_NT_on_D_off->addOutput(D_off);

  FuzzyRuleConsequent* then_WT_off_NT_off_D_on = new FuzzyRuleConsequent();
  then_WT_on_NT_on_D_off->addOutput(WT_off);
  then_WT_on_NT_on_D_off->addOutput(NT_off);
  then_WT_on_NT_on_D_off->addOutput(D_on);

  FuzzyRuleConsequent* then_WT_off_NT_on_D_off = new FuzzyRuleConsequent();
  then_WT_on_NT_on_D_off->addOutput(WT_off);
  then_WT_on_NT_on_D_off->addOutput(NT_on);
  then_WT_on_NT_on_D_off->addOutput(D_off);

  FuzzyRuleConsequent* then_WT_off_NT_on_D_on = new FuzzyRuleConsequent();
  then_WT_on_NT_on_D_off->addOutput(WT_off);
  then_WT_on_NT_on_D_off->addOutput(NT_on);
  then_WT_on_NT_on_D_off->addOutput(D_on);

  FuzzyRuleConsequent* then_WT_on_NT_off_D_off = new FuzzyRuleConsequent();
  then_WT_on_NT_on_D_off->addOutput(WT_on);
  then_WT_on_NT_on_D_off->addOutput(NT_off);
  then_WT_on_NT_on_D_off->addOutput(D_off);

  FuzzyRuleConsequent* then_WT_on_NT_off_D_on = new FuzzyRuleConsequent();
  then_WT_on_NT_on_D_off->addOutput(WT_on);
  then_WT_on_NT_on_D_off->addOutput(NT_off);
  then_WT_on_NT_on_D_off->addOutput(D_on);

  FuzzyRuleConsequent* then_WT_on_NT_on_D_off = new FuzzyRuleConsequent();
  then_WT_on_NT_on_D_off->addOutput(WT_on);
  then_WT_on_NT_on_D_off->addOutput(NT_on);
  then_WT_on_NT_on_D_off->addOutput(D_off);

  FuzzyRuleConsequent* then_WT_on_NT_on_D_on = new FuzzyRuleConsequent();
  then_WT_on_NT_on_D_off->addOutput(WT_on);
  then_WT_on_NT_on_D_off->addOutput(NT_on);
  then_WT_on_NT_on_D_off->addOutput(D_on);

  // rule1
  FuzzyRuleAntecedent* if_pH_Low_Ec_Low_Wl_Low = new FuzzyRuleAntecedent();
  if_pH_Low_Ec_Low_Wl_Low->joinWithAND(pH_low, EC_low, WL_low);

  FuzzyRule* rule1 = new FuzzyRule(1, if_pH_Low_Ec_Low_Wl_Low, then_WT_on_NT_on_D_off);
  fuzzy->addFuzzyRule(rule1);

  // 2
  FuzzyRuleAntecedent* if_PH_Low_Ec_Low_Wl_Within = new FuzzyRuleAntecedent();
  if_PH_Low_Ec_Low_Wl_Within->joinWithAND(pH_low, EC_low, WL_within);

  FuzzyRule* rule2 = new FuzzyRule(2, if_PH_Low_Ec_Low_Wl_Within, then_WT_on_NT_on_D_off);
  fuzzy->addFuzzyRule(rule2);

  // 3
  FuzzyRuleAntecedent* if_PH_Low_Ec_Low_Wl_Full = new FuzzyRuleAntecedent();
  if_PH_Low_Ec_Low_Wl_Full->joinWithAND(pH_low, EC_low, WL_full);

  FuzzyRule* rule3 = new FuzzyRule(3, if_PH_Low_Ec_Low_Wl_Within, then_WT_on_NT_on_D_on);
  fuzzy->addFuzzyRule(rule3);

  // 4
  FuzzyRuleAntecedent* if_pH_Low_Ec_Within_Wl_Low = new FuzzyRuleAntecedent();
  if_pH_Low_Ec_Within_Wl_Low->joinWithAND(pH_low, EC_within, WL_low);

  FuzzyRule* rule4 = new FuzzyRule(4, if_pH_Low_Ec_Within_Wl_Low, then_WT_on_NT_off_D_off);
  fuzzy->addFuzzyRule(rule4);

  // 5
  FuzzyRuleAntecedent* if_PH_Low_Ec_Within_Wl_Within = new FuzzyRuleAntecedent();
  if_PH_Low_Ec_Within_Wl_Within->joinWithAND(pH_low, EC_within, WL_within);

  FuzzyRule* rule5 = new FuzzyRule(5, if_PH_Low_Ec_Within_Wl_Within, then_WT_of_NT_off_D_off);
  fuzzy->addFuzzyRule(rule5);

  // 6
  FuzzyRuleAntecedent* if_PH_Low_Ec_Within_Wl_Full = new FuzzyRuleAntecedent();
  if_PH_Low_Ec_Within_Wl_Full->joinWithAND(pH_low, EC_within, WL_full);

  FuzzyRule* rule6 = new FuzzyRule(6, if_PH_Low_Ec_Within_Wl_Full, then_WT_on_NT_off_D_on);
  fuzzy->addyFuzzyRule(rule6);

  // 7
  FuzzyRuleAntecedent* if_pH_Low_Ec_High_Wl_Low = new FuzzyRuleAntecedent();
  if_pH_Low_Ec_High_Wl_Low->joinWithAND(pH_low, EC_High, WL_low);

  FuzzyRule* rule7 = new FuzzyRule(7, if_pH_Low_Ec_High_Wl_Low, then_WT_on_NT_off_D_off);
  fuzzy->addFuzzyRule(rule7);

  // 8
  FuzzyRuleAntecedent* if_PH_Low_Ec_High_Wl_Within = new FuzzyRuleAntecedent();
  if_PH_Low_Ec_High_Wl_Within->joinWithAND(pH_low, EC_high, WL_within);

  FuzzyRule* rule8 = new FuzzyRule(8, if_PH_Low_Ec_High_Wl_Within, then_WT_on_NT_off_D_off);
  fuzzy->addFuzzyRule(rule8);

  // 9
  FuzzyRuleAntecedent* if_PH_Low_Ec_high_Wl_Full = new FuzzyRuleAntecedent();
  if_PH_Low_Ec_high_Wl_Full->joinWithAND(pH_low, EC_high, WL_full);

  FuzzyRule* rule9 = new FuzzyRule(9, if_PH_Low_Ec_high_Wl_Full, then_WT_on_NT_off_D_on);
  fuzzy->addyFuzzyRule(rule9);

  // 10
  FuzzyRuleAntecedent* if_pH_Within_Ec_Low_Wl_Low = new FuzzyRuleAntecedent();
  if_pH_Within_Ec_Low_Wl_Low->joinWithAND(pH_within, EC_low, WL_low);

  FuzzyRule* rule10 = new FuzzyRule(10, if_pH_Within_Ec_Low_Wl_Low, then_WT_off_NT_on_D_off);
  fuzzy->addFuzzyRule(rule10);

  // 11
  FuzzyRuleAntecedent* if_PH_Within_Ec_Low_Wl_Within = new FuzzyRuleAntecedent();
  if_PH_Within_Ec_Low_Wl_Within->joinWithAND(pH_within, EC_within, WL_within);

  FuzzyRule* rule11 = new FuzzyRule(11, if_PH_Within_Ec_Low_Wl_Within, then_WT_off_NT_on_D_off);
  fuzzy->addFuzzyRule(rule11);

  // 12
  FuzzyRuleAntecedent* if_PH_Within_Ec_Low_Wl_Full = new FuzzyRuleAntecedent();
  if_PH_Within_Ec_Low_Wl_Full->joinWithAND(pH_within, EC_within, WL_full);

  FuzzyRule* rule12 = new FuzzyRule(12, if_PH_Within_Ec_Low_Wl_Full, then_WT_off_NT_on_D_on);
  fuzzy->addFuzzyRule(rule12);

  // 13
  FuzzyRuleAntecedent* if_pH_Within_Ec_Within_Wl_Low = new FuzzyRuleAntecedent();
  if_pH_Within_Ec_Within_Wl_Low->joinWithAND(pH_within, EC_within, WL_low);

  FuzzyRule* rule13 = new FuzzyRule(13, if_pH_Within_Ec_Within_Wl_Low, then_WT_off_NT_off_D_off);
  fuzzy->addFuzzyRule(rule13);

  // 14
  FuzzyRuleAntecedent* if_PH_Within_Ec_Within_Wl_Within = new FuzzyRuleAntecedent();
  if_PH_Within_Ec_Within_Wl_Within->joinWithAND(pH_within, EC_within, WL_within);

  FuzzyRule* rule14 = new FuzzyRule(14, if_PH_Within_Ec_Within_Wl_Within, then_WT_off_NT_off_D_off);
  fuzzy->addFuzzyRule(rule14);

  // 15
  FuzzyRuleAntecedent* if_PH_Within_Ec_Within_Wl_Full = new FuzzyRuleAntecedent();
  if_PH_Within_Ec_Within_Wl_Full->joinWithAND(pH_within, EC_within, WL_full);

  FuzzyRule* rule15 = new FuzzyRule(15, if_PH_Within_Ec_Within_Wl_Full, then_WT_off_NT_off_D_on);
  fuzzy->addFuzzyRule(rule15);

  // 16
  FuzzyRuleAntecedent* if_pH_Within_Ec_High_Wl_Low = new FuzzyRuleAntecedent();
  if_pH_Within_Ec_High_Wl_Low->joinWithAND(pH_within, EC_high, WL_low);

  FuzzyRule* rule16 = new FuzzyRule(16, if_pH_Within_Ec_High_Wl_Low, then_WT_off_NT_off_D_off);
  fuzzy->addFuzzyRule(rule16);

  // 17
  FuzzyRuleAntecedent* if_PH_Within_Ec_High_Wl_Within = new FuzzyRuleAntecedent();
  if_PH_Within_Ec_High_Wl_Within->joinWithAND(pH_within, EC_high, WL_within);

  FuzzyRule* rule17 = new FuzzyRule(17, if_PH_Within_Ec_High_Wl_Within, then_WT_off_NT_off_D_off);
  fuzzy->addFuzzyRule(rule17);

  // 18
  FuzzyRuleAntecedent* if_PH_Within_Ec_High_Wl_Full = new FuzzyRuleAntecedent();
  if_PH_Within_Ec_High_Wl_Full->joinWithAND(pH_within, EC_high, WL_full);

  FuzzyRule* rule18 = new FuzzyRule(18, if_PH_Within_Ec_High_Wl_Full, then_WT_off_NT_off_D_on);
  fuzzy->addFuzzyRule(rule18);

  // 19
  FuzzyRuleAntecedent* if_pH_High_Ec_Low_Wl_Low = new FuzzyRuleAntecedent();
  if_pH_High_Ec_Low_Wl_Low->joinWithAND(pH_high, EC_low, WL_low);

  FuzzyRule* rule19 = new FuzzyRule(19, if_pH_High_Ec_Low_Wl_Low, then_WT_off_NT_on_D_off);
  fuzzy->addFuzzyRule(rule19);

  // 20
  FuzzyRuleAntecedent* if_PH_High_Ec_Low_Wl_Within = new FuzzyRuleAntecedent();
  if_PH_High_Ec_Low_Wl_Within->joinWithAND(pH_high, EC_within, WL_within);

  FuzzyRule* rule20 = new FuzzyRule(20, if_PH_High_Ec_Low_Wl_Within, then_WT_off_NT_on_D_off);
  fuzzy->addFuzzyRule(rule20);

  // 21
  FuzzyRuleAntecedent* if_PH_High_Ec_Low_Wl_Full = new FuzzyRuleAntecedent();
  if_PH_High_Ec_Low_Wl_Full->joinWithAND(pH_high, EC_within, WL_full);

  FuzzyRule* rule21 = new FuzzyRule(21, if_PH_High_Ec_Low_Wl_Full, then_WT_off_NT_on_D_on);
  fuzzy->addFuzzyRule(rule21);

  // 22
  FuzzyRuleAntecedent* if_pH_High_Ec_Within_Wl_Low = new FuzzyRuleAntecedent();
  if_pH_High_Ec_Within_Wl_Low->joinWithAND(pH_high, EC_within, WL_low);

  FuzzyRule* rule22 = new FuzzyRule(22, if_pH_High_Ec_Within_Wl_Low, then_WT_off_NT_off_D_off);
  fuzzy->addFuzzyRule(rule22);

  // 23
  FuzzyRuleAntecedent* if_PH_High_Ec_Within_Wl_Within = new FuzzyRuleAntecedent();
  if_PH_High_Ec_Within_Wl_Within->joinWithAND(pH_high, EC_within, WL_within);

  FuzzyRule* rule23 = new FuzzyRule(23, if_PH_High_Ec_Within_Wl_Within, then_WT_off_NT_off_D_off);
  fuzzy->addFuzzyRule(rule23);

  // 24
  FuzzyRuleAntecedent* if_PH_High_Ec_Within_Wl_Full = new FuzzyRuleAntecedent();
  if_PH_High_Ec_Within_Wl_Full->joinWithAND(pH_high, EC_within, WL_full);

  FuzzyRule* rule15 = new FuzzyRule(24, if_PH_High_Ec_Within_Wl_Full, then_WT_off_NT_off_D_on);
  fuzzy->addFuzzyRule(rule24);

  // 25
  FuzzyRuleAntecedent* if_pH_High_Ec_High_Wl_Low = new FuzzyRuleAntecedent();
  if_pH_High_Ec_High_Wl_Low->joinWithAND(pH_high, EC_high, WL_low);

  FuzzyRule* rule25 = new FuzzyRule(25, if_pH_High_Ec_High_Wl_Low, then_WT_off_NT_off_D_off);
  fuzzy->addFuzzyRule(rule25);

  // 26
  FuzzyRuleAntecedent* if_PH_High_Ec_High_Wl_Within = new FuzzyRuleAntecedent();
  if_PH_High_Ec_High_Wl_Within->joinWithAND(pH_high, EC_high, WL_within);

  FuzzyRule* rule26 = new FuzzyRule(26, if_PH_High_Ec_High_Wl_Within, then_WT_off_NT_off_D_off);
  fuzzy->addFuzzyRule(rule26);

  // 27
  FuzzyRuleAntecedent* if_PH_High_Ec_High_Wl_Full = new FuzzyRuleAntecedent();
  if_PH_High_Ec_High_Wl_Full->joinWithAND(pH_high, EC_high, WL_full);

  FuzzyRule* rule27 = new FuzzyRule(27, if_PH_High_Ec_High_Wl_Full, then_WT_off_NT_off_D_on);
  fuzzy->addFuzzyRule(rule27);
}

void set_fuzzy_inputs(float f_pH, float f_ec, float f_wl) {
  // define input object
  fuzzy->setInput(1, f_pH);
  fuzzy->setInput(2, f_ec);
  fuzzy->setInput(3, f_wl);
}

void fuzzify_sys() {
  fuzzy->fuzzify();
}

fuzzyoutputs defuzzify_sys() {
  fuzzyoutputs outputs;
  outputs.i_wt = fuzzy->defuzzify(1);
  outputs.i_nt = fuzzy->defuzzify(2);
  outputs.i_nt = fuzzy->defuzzify(3);
  return outputs;
}
#endif  // __FUZZY_H__