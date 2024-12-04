//Expert system for Vehicle Diagnostic Expert System

% Knowledge Base: Vehicle Diagnostic Expert System

% Facts: Symptoms and corresponding solutions for various vehicle issues.

% Engine issues
symptom(engine, "engine won't start", [
    "Check if the battery is charged.",
    "Inspect the fuel level.",
    "Ensure the ignition switch is working."
]).
symptom(engine, "engine is overheating", [
    "Check the coolant level.",
    "Inspect the radiator for leaks.",
    "Ensure the water pump is functional."
]).
symptom(engine, "engine is making noise", [
    "Check the engine oil level.",
    "Inspect the timing belt.",
    "Ensure the spark plugs are in good condition."
]).

% Brake issues
symptom(brakes, "squeaking brakes", [
    "Check for worn brake pads.",
    "Inspect the brake rotor.",
    "Ensure the brake calipers are functioning properly."
]).
symptom(brakes, "brakes feel spongy", [
    "Check the brake fluid level.",
    "Inspect for air in the brake lines.",
    "Ensure the brake master cylinder is operational."
]).
symptom(brakes, "brakes are not responsive", [
    "Check for brake fluid leaks.",
    "Inspect the brake booster.",
    "Ensure brake pads are not excessively worn."
]).

% Battery issues
symptom(battery, "battery won't charge", [
    "Check the alternator.",
    "Inspect the battery terminals for corrosion.",
    "Ensure the battery is not old or faulty."
]).
symptom(battery, "dim headlights", [
    "Check the battery charge.",
    "Inspect the alternator and wiring.",
    "Ensure the headlight bulbs are functional."
]).
symptom(battery, "car won't start", [
    "Check the battery charge.",
    "Inspect the ignition system.",
    "Ensure the starter motor is working."
]).

% Tire issues
symptom(tires, "low tire pressure", [
    "Inflate the tires to the recommended pressure.",
    "Inspect for punctures or leaks.",
    "Ensure the tire pressure monitoring system is functional."
]).
symptom(tires, "uneven tire wear", [
    "Check the wheel alignment.",
    "Inspect the tire tread depth.",
    "Ensure the suspension system is in good condition."
]).
symptom(tires, "vibrating steering wheel", [
    "Inspect for unbalanced tires.",
    "Check the wheel alignment.",
    "Ensure the suspension system is not damaged."
]).

% Rules: Main entry point with looping functionality
start :-
    write("Welcome to the AutoExpress Vehicle Diagnostic Expert System!"), nl,
    main_loop. % Start the main loop

% Main loop: Keeps running until the user chooses to exit
main_loop :-
    write("What would you like to do?"), nl,
    write("1. Diagnose a vehicle issue"), nl,
    write("2. Exit"), nl,
    read(Choice),
    handle_choice(Choice).

% Handle user choice: Perform actions based on user input
handle_choice(1) :- % Diagnose a vehicle issue
    diagnose,
    main_loop. % Return to the main loop after diagnosis
handle_choice(2) :- % Exit the program
    write("Thank you for using the AutoExpress Vehicle Diagnostic Expert System!"), nl,
    write("Goodbye!"), nl.
handle_choice(_) :- % Invalid input
    write("Invalid choice. Please try again."), nl,
    main_loop.

% Diagnosis process
diagnose :-
    write("Please select the type of issue you are experiencing:"), nl,
    list_categories(Categories),
    read(CategoryChoice),
    nth1(CategoryChoice, Categories, SelectedCategory),
    write("You selected: "), write(SelectedCategory), nl,
    write("Please select the symptom that matches your problem:"), nl,
    list_symptoms(SelectedCategory, Symptoms),
    read(SymptomChoice),
    nth1(SymptomChoice, Symptoms, SelectedSymptom),
    write("You selected: "), write(SelectedSymptom), nl,
    write("Here are some suggestions to address the issue:"), nl,
    symptom(SelectedCategory, SelectedSymptom, Solutions),
    display_solutions(Solutions).

% Helper predicates

% List all available categories
list_categories(Categories) :-
    findall(Category, symptom(Category, _, _), AllCategories),
    sort(AllCategories, Categories), % Ensure no duplicates
    list_options(Categories).

% List symptoms for a given category
list_symptoms(Category, Symptoms) :-
    findall(Symptom, symptom(Category, Symptom, _), Symptoms),
    list_options(Symptoms).

% Display numbered options for a list
list_options(Options) :-
    list_options(Options, 1).

list_options([], _).
list_options([Option|Rest], N) :-
    write(N), write(". "), write(Option), nl,
    N1 is N + 1,
    list_options(Rest, N1).

% Display solutions
display_solutions([]).
display_solutions([Solution|Rest]) :-
    write("- "), write(Solution), nl,
    display_solutions(Rest).
