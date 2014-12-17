var currentStep = 1;

function getCurrentStep() {
  return currentStep;
}

function setCurrentStep(value) {
  currentStep = value;
}

function incrementStep() {
  currentStep += 1;
  updateQMLWidgets();
}

function decrementStep() {
  currentStep -= 1;
  updateQMLWidgets();
}

function updateQMLWidgets() {
  nextStepMouseArea.width = StepJS.getCurrentStep() < 3 ? 40 : 0;
  nextStepMouseArea.height = StepJS.getCurrentStep() < 3 ? 40 : 0;
  nextText.visible = StepJS.getCurrentStep() < 3;

  backStepMouseArea.width = StepJS.getCurrentStep() > 1 ? 40 : 0;
  backStepMouseArea.height = StepJS.getCurrentStep() > 1 ? 40 : 0;
  backText.visible = StepJS.getCurrentStep() > 1;

  step1Text.color = StepJS.getCurrentStep() >= 1 ? "steelblue" : "lightgrey"
  step2Text.color = StepJS.getCurrentStep() >= 2 ? "steelblue" : "lightgrey";
  step3Text.color = StepJS.getCurrentStep() >= 3 ? "steelblue" : "lightgrey";

  step2Rectangle.color = StepJS.getCurrentStep() >= 2 ? "steelblue" : "lightgrey"
  step3Rectangle.color = StepJS.getCurrentStep() >= 3 ? "steelblue" : "lightgrey"

  step1Step2Rectangle.color = StepJS.getCurrentStep() >= 2 ? "steelblue" : "lightgrey"
  step2Step3Rectangle.color = StepJS.getCurrentStep() >= 3 ? "steelblue" : "lightgrey"
}
