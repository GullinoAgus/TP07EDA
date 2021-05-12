#include <iostream>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_color.h>
#include <string>
#include <ctime>
#include "imgui.h"
#include "imgui_impl_allegro5.h"
#include "basicLCD.h"
#include "Class_LUCIANO.C/DispL.h"
#include "TweetDLoader.h"
using namespace std;
//enum{IDLE=0, DOWNLOADING, DISPLAYING};

#define DISP_W 500
#define DISP_H 500
#define FPS 10
#define PLAY_ICON u8"\xF452"
#define PAUSE_ICON u8"\xF448"
#define SPEEDUP_ICON u8"\xF458"
#define SLOWDOWN_ICON u8"\xF459"
#define FORWARD_ICON u8"\xF451"
#define BACKWARD_ICON u8"\xF450"
#define REPLAY_ICON u8"\xF420"
#define DOWNLOAD_ICON u8"\xF440"
#define CANCEL_ICON u8"\xF405"
#define DEFAULT_SCROLL_SPEED 10
#define MIN_SCROLL_SPEED 1
#define MAX_SCROLL_SPEED 100

unsigned int refreshLCDs(basicLCD** LCD, bool* redo, bool pause, int state, string twits, int twit_cursor, int scroll_speed);


static void HelpMarker(const char* desc) {
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}



int main(int argc, char* argv[]) {
	bool err = false;

	// ---------- Inicializacion de Allegro. ----------



	if (al_init()) { //Dear ImGUI lo necesita.
		if (al_init_image_addon()) {
			if (al_init_font_addon()) {
				if (al_init_ttf_addon()) {
					if (al_init_primitives_addon()) { // Dear ImGui necesita Allegro para poder graficar. Para esto, usa el add-on de primitivas.


						ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
						if (!!queue) {
							if (al_install_keyboard()) {
								al_register_event_source(queue, al_get_keyboard_event_source());
								if (al_install_mouse()) {
									al_register_event_source(queue, al_get_mouse_event_source());
									ALLEGRO_TIMER* timerA = nullptr;
									timerA = al_create_timer(1.0 / FPS);
									if (!!timerA) {


										ALLEGRO_DISPLAY* display = nullptr;
										display = al_create_display(DISP_W, DISP_H);





										al_register_event_source(queue, al_get_display_event_source(display));
										al_register_event_source(queue, al_get_timer_event_source(timerA));

										basicLCD* LCD[3] = { nullptr,nullptr,nullptr };
										//TODO: BUILD TWITTER DOWNLOADER TRY/CATCH
										TweetDLoader twitterDL("HCB39Q15wIoH61KIkY5faRDf6", "7s8uvgQnJqjJDqA6JsLIFp90FcOaoR5Ic41LWyHOic0Ht3SRJ6");
										list<string>twitList;

										al_set_target_backbuffer(display);
										//ALLEGRO_FONT* font = nullptr;
										//font = al_load_ttf_font(FONT, FONT_SIZE, 0);
										// Setup de Dear ImGui
										IMGUI_CHECKVERSION();			// Control de version de Dear ImGui
										ImGui::CreateContext();
										al_set_target_backbuffer(display);
										ALLEGRO_DISPLAY* dispi = al_get_current_display();
										ImGui_ImplAllegro5_Init(display); // Inicializa Dear ImGui
										ImGuiIO& io = ImGui::GetIO();	// Me permite obtener informacion y cambiar parametros del mouse y de teclado.
										ImGui::StyleColorsDark();		// Opcional: cambiar la paleta de colores default por otra
										static const ImWchar icons_ranges[] = { 0xf000, 0xf5ff, 0 }; // Will not be copied by AddFont* so keep in scope.
										ImFontConfig config;
										config.MergeMode = true;
										io.Fonts->AddFontFromFileTTF("arista.ttf", 21);
										io.Fonts->AddFontFromFileTTF("Genericons-Neue.ttf", 25, &config, icons_ranges);
										io.Fonts->Build();

										//Inicializo variables.

										bool running = true;			// Exit flag, upon main display close
										int state = IDLE;
										char screen_name[15] = { };
										int twit_qnt = 1;
										int lcd_on[3] = { 0, 0, 0 }; //es int porque sino el overload de la funcion de IMGUI no anda, pero actua como bool
										bool download_req = false;
										int scroll_speed = DEFAULT_SCROLL_SPEED;
										bool pause = false;
										bool redo = false;
										int forward = 0;
										int backward = 0;
										unsigned int curr_twit = 0;
										string placeholder = " ";

										//variables auxiliares que usan los widgets	
										bool sim_help = false;
										bool pre_help = false;
										bool refresh = true;
										bool twit_qnt_override = false;




										//IMGUI variables
										ImGuiWindowFlags window_flags = 0;
										window_flags |= ImGuiWindowFlags_NoBackground;
										window_flags |= ImGuiWindowFlags_NoTitleBar;
										window_flags |= ImGuiWindowFlags_NoResize;
										window_flags |= ImGuiWindowFlags_NoCollapse;
										window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
										window_flags |= ImGuiWindowFlags_NoSavedSettings;
										window_flags |= ImGuiWindowFlags_NoMove;
										ImGuiSliderFlags reg_slider = ImGuiSliderFlags_AlwaysClamp;
										ImGuiSliderFlags log_slider = reg_slider | ImGuiSliderFlags_Logarithmic;
										const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
										al_start_timer(timerA);
										/*RUNNING STARTS HERE*/			while (running)
										{

											ALLEGRO_EVENT ev;
											while (al_get_next_event(queue, &ev))
											{
												ImGui_ImplAllegro5_ProcessEvent(&ev);	// Mandar el evento a Dear ImGui para que lo procese

												if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
													running = false;
												// io.WantCaptureKeyboard devuelve true si el ultimo evento fue de teclado y ademas fue 
												// usado por el usuario para interactuar con las ventanas de Dear Imgui. Por este motivo, 
												// sabemos que no estaba dirigido a nuestra aplicacion principal y no debemos mandarselo a 
												// su dispatcher. Idem para io.WantCaptureMouse y los eventos de mouse
												if (ev.type == ALLEGRO_EVENT_TIMER) {

													if (!lcd_on[0] ^ !LCD[0]) { // XOR, if they are not the same, goes into the function.
														if (LCD[0] != nullptr) {
															delete LCD[0];
															LCD[0] = nullptr;
														}
														else {
															LCD[0] = new DispL;
															if (!LCD[0]->lcdInitOk()) {
																delete LCD[0];
																cout << "Error in lcd init" << endl;
															}
														}
													}
													if (!lcd_on[1] ^ !LCD[1]) {
														if (LCD[1] != nullptr) {
															delete LCD[1];
														}
														else {
															//LCD[1]= new YY;
														}
													}
													if (!lcd_on[2] ^ !LCD[2]) {
														if (LCD[2] != nullptr) {
															//delete LCD[2];
														}
														else {
															//LCD[2]=new XX;
														}
													}


													//Update variables for frame refresh
													if (state == IDLE)
														if (download_req) {
															state = DOWNLOADING;
															//Start downloading with current screen_name and Twits quantity
															twitterDL.download(twitList, screen_name, twit_qnt);
														}
													if (state == DOWNLOADING) {
														if (!download_req) {
															twitterDL.cancelDownload();
															state = DISPLAYING;
														}
														else {
															if (twitterDL.getStatus() != DOWNLOADING)
																state = DISPLAYING;
														}
													}
													if (state == DISPLAYING) {
														if (download_req) {
															state = DOWNLOADING;
															twitterDL.download(twitList, screen_name, twit_qnt);//Start downloading with current screen_name and Twits quantity
														}
														if ((forward - backward) != 0) {
															curr_twit += (forward - backward);
															if (curr_twit < 0)
																curr_twit = 0;
															if (curr_twit > twitList.size())  //Ultimo twit
																curr_twit = twitList.size();
														}
													}
													refresh = true;
												}
												if (!io.WantCaptureKeyboard && !io.WantCaptureMouse)
												{
													// Dispatch a nuestra aplicacion principal (puede usarse para agregar teclado en la GUI)
												}
											}

											// Inicio el frame. Se realiza una vez por cada pantalla que dibujo.
											ImGui_ImplAllegro5_NewFrame();
											ImGui::NewFrame();



											ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 100, main_viewport->WorkPos.y + 25), ImGuiCond_FirstUseEver);
											ImGui::Begin("Open statement:", NULL, window_flags);                          // Create a window for sliders																					
											ImGui::Text("Twitter LCD Presenter");
											ImGui::SameLine(); HelpMarker("Este programa muestra Twits de un usuario de Twitter\nen 3 posibles displays diferentes.\n Utilice los (?) info helpers para mas\ninformacion sobre cada widget.");
											ImGui::End();

											ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 0, main_viewport->WorkPos.y + 0), ImGuiCond_FirstUseEver);
											ImGui::Begin("TP ID:", NULL, window_flags);                          // Create a window for sliders																					
											ImGui::Text("TP7 - Grupo 5");
											//ImGui::SameLine(); HelpMarker("Welcome to our program.");
											ImGui::End();

											//TextBox
											// To wire InputText() with std::string or any other custom string type,
											// see the "Text Input > Resize Callback" section of this demo, and the misc/cpp/imgui_stdlib.h file.
											ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 0, main_viewport->WorkPos.y + 125), ImGuiCond_FirstUseEver);
											ImGui::Begin("Twitter Username Text box:", NULL, window_flags);                          // Create a window for sliders																					
											ImGui::InputTextWithHint("", "Twitter Handle goes here.", screen_name, IM_ARRAYSIZE(screen_name));
											ImGui::SameLine(); HelpMarker(
												"USER:\n"
												"Hold SHIFT or use mouse to select text.\n"
												"CTRL+Left/Right to word jump.\n"
												"CTRL+A or double-click to select all.\n"
												"CTRL+X,CTRL+C,CTRL+V clipboard.\n"
												"CTRL+Z,CTRL+Y undo/redo.\n"
												"ESCAPE to revert.\n\n"
											);
											ImGui::End();

											//Checkbox and slider
											ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 0, main_viewport->WorkPos.y + 155), ImGuiCond_FirstUseEver);
											ImGui::Begin("Is # of twits default:", NULL, window_flags);                          // Create a window for sliders																					
											ImGui::Checkbox("Specify number of Twits to Download?", &twit_qnt_override);
											if (twit_qnt_override) {
												ImGui::SliderInt("# of Twits\nto Download", &twit_qnt, 1, 3200, NULL, reg_slider);		   // Slider for Twit quantity
												ImGui::SameLine(); HelpMarker("CTRL+click to input value.\nOtherwise use slider.");
											}
											ImGui::End();

											//LCD Buttons 1 - 2 - 3
											ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 115, main_viewport->WorkPos.y + 235), ImGuiCond_FirstUseEver);
											ImGui::Begin("LCD ON/OFF switches:", NULL, window_flags);                          // Create a window for sliders																					
											ImGui::RadioButton("LCD 1", &lcd_on[0], !lcd_on[0]); ImGui::SameLine();
											ImGui::RadioButton("LCD 2", &lcd_on[1], !lcd_on[1]); ImGui::SameLine();
											ImGui::RadioButton("LCD 3", &lcd_on[2], !lcd_on[2]);
											ImGui::SameLine(); HelpMarker(
												"Turn any LCD on/off for Twit viewing."
											);
											ImGui::End();

											//Download Button - Cancel Download Button
											ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 155, main_viewport->WorkPos.y + 275), ImGuiCond_FirstUseEver);
											ImGui::Begin("Download button:", NULL, window_flags);                          // Create a window for sliders																					
											if (download_req) {
												if (ImGui::Button("Cancel Download\n             " CANCEL_ICON))                            // Buttons return true when clicked (most widgets return true when edited/activated)
													download_req = false;
												ImGui::SameLine(); HelpMarker(
													"Click to stop downloading prematurely and show twits."
													"\nTwits will show when download finishes."
												);
											}
											else {
												if (ImGui::Button(" Start Download \n             " DOWNLOAD_ICON))                            // Buttons return true when clicked (most widgets return true when edited/activated)
													download_req = true;
												ImGui::SameLine(); HelpMarker(
													"Click to start downloading twits.\n"
													"Make sure the handle and number\nof twits are selected first.\n"
													"Twits will show when download ends,\n"
													"either when finished or cancelled."
												);
											}
											ImGui::End();

											ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 155, main_viewport->WorkPos.y + 500 - 140), ImGuiCond_FirstUseEver);
											ImGui::Begin("text:", NULL, window_flags);                          // Create a window for sliders																					
											ImGui::Text("Display controls:");
											ImGui::End();

											//SpeedUp and SlowDown Buttons
											ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 15, main_viewport->WorkPos.y + 500 - 115), ImGuiCond_FirstUseEver);
											ImGui::Begin("Speed Controls:", NULL, window_flags);                          // Create a window for sliders																					
											ImGui::PushButtonRepeat(true);
											if (ImGui::Button(SLOWDOWN_ICON /*"\nSlow Down"*/)) { scroll_speed--; if (scroll_speed < MIN_SCROLL_SPEED) scroll_speed = MIN_SCROLL_SPEED; }
											ImGui::SameLine();
											if (ImGui::Button(SPEEDUP_ICON /*"\nSpeed Up"*/)) { scroll_speed++;  if (scroll_speed > MAX_SCROLL_SPEED) scroll_speed = MAX_SCROLL_SPEED; }
											ImGui::PopButtonRepeat();
											ImGui::SameLine();
											ImGui::SameLine(); ImGui::SliderInt(" ", &scroll_speed, 1, 100, NULL, reg_slider);		   // Slider for Twit quantity
											ImGui::SameLine();
											ImGui::SameLine(); HelpMarker(
												"LCD scrolling speed.\n"
												"Use buttons or slider.\n"
												"CTRL+click to input value.\n");
											ImGui::End();

											//Back - Redo - Pause/Play - Forward buttons
											ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 105, main_viewport->WorkPos.y + 500 - 80), ImGuiCond_FirstUseEver);
											ImGui::Begin("LCD display controls:", NULL, window_flags);                          // Create a window for sliders																					
											if (ImGui::Button(" " BACKWARD_ICON "\nPrev")) { backward++; }
											ImGui::SameLine(); if (ImGui::Button("    " REPLAY_ICON "\nReplay")) { if (state == DISPLAYING) redo = true; }
											ImGui::SameLine(); if (pause) {
												if (ImGui::Button("   " PLAY_ICON "\n Play  "))
													pause = false;
											}
											else
												if (ImGui::Button("   " PAUSE_ICON "\nPause"))
													pause = true;
											ImGui::SameLine(); if (ImGui::Button("  " FORWARD_ICON "\nNext")) { forward++; }
											ImGui::SameLine(); HelpMarker(
												"Display controls.\n"
												"Prev: Shows previous Twit.\n"
												"Replay: Restart current Twit.\n"
												"Next: Skips to next Twit.\n"
												"Applies to all LCD equally.\n"
											);
											ImGui::End();

											ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 0, main_viewport->WorkPos.y + 500 - 30), ImGuiCond_FirstUseEver);
											ImGui::Begin("Debug:", NULL, window_flags);                          // Create a window for fps view																				
											ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
											ImGui::End();

											if ((state == DOWNLOADING) && download_req) {
												if (twitterDL.getStatus() != DOWNLOADING) {
													download_req = 0;
												}
												else if (!twitterDL.download(twitList, screen_name, twit_qnt))
													download_req = 0;
											}

											// Rendering
											ImGui::Render();	//No dibuja! Solamente calcula que es lo que tiene que dibujarse

																// Puedo usar funciones de dibujo de Allegro ademas de las de 
																// ImGui.
																// Todo lo que dibuje antes de ImGui_ImplAllegro5_RenderDrawData
																// va a quedar detras de las ventanas de DearImGui, y todo lo 
																// que dibuje despues va a quedar encima de las ventanas de 
																// DearImGui.


																//Todo lo que dibuje aca va a quedar por detrás de las ventanas de DearImGui
											al_set_target_backbuffer(display);
											al_clear_to_color(al_map_rgb(0, 0, 0));

											ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());	//Dibuja las ventanas, pero no hace al_flip_display()

																										//Todo lo que dibuje aca va a quedar por encima de las ventanas de DearImGui
											if (refresh) {

												curr_twit = refreshLCDs(LCD, &redo, pause, state, placeholder/*twits*/, curr_twit, scroll_speed);

												al_flip_display(); //DearImGui nunca hace al_flip_display()
												refresh = false;
											}
											/*RUNNING ENDS HERE*/
										}

										// Cleanup final
										ImGui_ImplAllegro5_Shutdown();
										ImGui::DestroyContext();



										al_destroy_display(display);
										for (int i = 0; i < 3; i++) {
											if (LCD[i] != nullptr) {
												delete LCD[i];
											}
										}

									}
									else { cout << "failed to create timer!\n" << endl; err = true; }
								}
								else { err = true; printf("Failed to install mouse!\n"); }
							}
							else { err = true; printf("Failed to install keyboard!\n"); }
							al_destroy_event_queue(queue);
						}
						else { err = true; printf("Failed to initialize Allegro event queue!\n"); }
					}
					else { err = true; printf("Failed to initialize primitives Add_on!\n"); }
				}
				else { err = true; printf("Failed to initialize ttf Add_on!\n"); }
			}
			else { err = true; printf("Failed to initialize font Add_on!\n"); }
		}
		else { err = true; printf("Failed to initialize image Add_on!\n"); }
	}
	else { err = true; printf("Failed to initialize Allegro!\n"); }
	return err;
}

unsigned int refreshLCDs(basicLCD** LCD, bool* redo, bool pause, int state, string twits, int twit_cursor, int scroll_speed) {
	static unsigned int curr_twit;
	const cursorPosition second_line = { MAX_ROW,MIN_COL };
	if (state != IDLE) { //if IDLE, do nothing
		string spaces = "                ";
		static int offset = 0;
		static int counter = 0;
		//int waiting = MAX_SCROLLSPEED - scroll_speed;
		//if(counter>waiting){
		//counter=0;
		if (state == DOWNLOADING) {
			//Do download cycle
			string downloading = "Downloading....";
			for (int i = 0; i < 3; i++) {
				if (LCD[i] != nullptr)
				{
					LCD[i]->lcdClear();
					//if(twits.screen_name.size()=<MAX_COL)
						//*LCD[i] << twits.screen_name();
					//else
						//*LCD[i] << twits.screen_name[offset];
					LCD[i]->lcdSetCursorPosition(second_line);
					*LCD[i] << (const unsigned char*)((spaces + downloading).substr(offset, string::npos)).c_str();
					offset++;
					if (offset == (16 * 2))
						offset = 0;
				}
			}
		}
		else {
			//if state displaying
			if (twit_cursor != curr_twit) {
				curr_twit = twit_cursor;
				offset = 0;
			}
			if (*redo) {
				offset = 0;
				*redo = 0;
			}
			if (!pause) {
				//if(curr_twit<twit.len){
				//	
				//
				for (int i = 0; i < 3; i++) {
					if (LCD[i] != nullptr)
					{
						LCD[i]->lcdClear();
						//*LCD[i] << twits.substr(0,16);
						LCD[i]->lcdSetCursorPosition(second_line);
						//*LCD[i] << (const unsigned char*)((twit.substr(16,string::npos)).substr(offset, string::npos)).c_str();
						//offset++;
						//if (offset == (twit.substr(16,string::npos)).size()){
						//	offset = 0;
						//	curr_twit++;
						//}
					}
					//}else{
					/*offset=0;
					for (int i = 0; i < 3; i++) {
					if (LCD[i] != nullptr)
					{
						LCD[i]->lcdClear();
						LCD[i]->lcdSetCursorPosition(second_line);
						//*LCD[i] << "Ultimo Twit.";
					}
					*/
				}
			}

			//else load current twit while scrolling

		}
		//}else{
		//	counter++;
		//}
	}
	return curr_twit; //return curr_twit;
}

/*ImGui::PushID(i);
ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
ImGui::Button("Click");
ImGui::PopStyleColor(3);
ImGui::PopID();*/
//ImGui::AlignTextToFramePadding();