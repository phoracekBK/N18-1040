#include "lang.h"



lang ** multi_lang;




void lang_init_czech();
void lang_init_english();




lang * multi_lang_get(int language)
{
	if((language >= 0) && (language < lang_num))
		return multi_lang[language];
	else
		return NULL;
}

void lang_init_czech(lang * cz_lang)
{    
	/* Czech labels */
    cz_lang->lang_name = "Čeština";
    cz_lang->win_title = "Host-BK CZ";
    cz_lang->gui_job_state = "Aktuální stav";
    cz_lang->gui_job_name = "Název-ID jobu";
    cz_lang->gui_job_order = "Index jobu";
    cz_lang->gui_job_sheet_number = "Celkový počet archů";
    cz_lang->gui_job_stamp_number = "Celkový počet kolků";
    cz_lang->gui_job_date_time = "Datum a čas";
    cz_lang->print_state_print = "Tisk";
    cz_lang->print_state_job_error = "Job v chybě";
    cz_lang->print_state_wait = "Čeká se na start";
    cz_lang->print_state_wait_for_data = "Čeká se na data";
    cz_lang->set_btn_network = "Nastavení sítě";
    cz_lang->set_btn_lang_settings = "Nastavení jazyka";
    cz_lang->set_btn_print_param = "Tiskové parametry";
    cz_lang->set_btn_hotfolder = "Nastavení hotfolderů";
    cz_lang->set_btn_io_vision = "Zobrazení IO";
    cz_lang->set_btn_back = "Zpět na controlní panel";
    cz_lang->set_btn_back_to_settngs = "Zpět";

    cz_lang->err_no_error = "Připraven";
    cz_lang->err_cant_save_f_csv = "Nelze uložit zpětnovazební csv!";
    cz_lang->err_analyze_camera_csv = "Chyba analýzy camerového csv!";
    cz_lang->err_cant_load_bkcore_csv = "Nelze načíst řídící csv!";
    cz_lang->err_cant_copy_pdf_to_gis = "Nelze odeslat pdf soubor do GISu!";
    cz_lang->err_cant_copy_csv_to_camera = "Nelze odeslat csv soubor do kamery!";
    cz_lang->err_job_order_mismatch = "Nesoulad pořadových indexů jobů!";
    cz_lang->err_stacker_jam_pile = "Vykladač zahlcen!";
    cz_lang->err_stacker_jam_conveyor = "Vykladač ucpán!";
    cz_lang->err_stacker_bottom = "Vykladač není v pracovní pozici!";
    cz_lang->err_stacker_multifunction = "Vykladač v chybovém stavu!";
    cz_lang->err_stacker_off = "Vykladač není aktivní!";
    cz_lang->err_feeder_e_stop = "Podavač hlásí E-stop!";
    cz_lang->err_feeder_off = "Podavač není aktivní!";
    cz_lang->err_feeder_multifunction = "Podavač v chybovém stavu!";
    cz_lang->err_feeder_sheet_missing = "Chybějící arch!";
    cz_lang->err_feeder_double_sheet = "Dvojitý arch!";
    cz_lang->err_machine_e_stop = "Stroj hlásí E-stop!";
    cz_lang->err_reject_bin_jam = "Výhybka ucpána!";
    cz_lang->err_feeder_jam = "Podavač ucpán!";
    cz_lang->err_ti = "Podavač nastřelovacích proužků hlásí chybu!";
    cz_lang->err_ta = "Podavač prokladových archů hlásí chybu!";
    cz_lang->err_counters_mismatch = "Nesoulad počítadel!";
    cz_lang->err_low_print_quality = "Překročen maximální počet po sobě vyhozených archů!";
    cz_lang->err_gis_disconnected = "Počítač GISu neodpovídá!";
    cz_lang->err_csv_sheet_num = "Chybné pořadí csv archů!"; 
    cz_lang->err_sheet_feeder_redirection = "Chyba při přesměrování nakladače pro prokladový arch!";
    cz_lang->err_pci_computer_not_responding = "Počítač PCI neodpovídá!";
    cz_lang->err_iij_computer_not_responding = "Počítač IIJ neodpovídá!";
    cz_lang->err_quadient_computer_not_responding = "Počítač Quadient neodpovídá!";
    cz_lang->err_print_main_freeze = "Nakládání hlavního archu je zablokováno";
    cz_lang->err_print_companion_freeze = "Nakládání prokladového archu je zablokováno";
    cz_lang->err_print_initialization_freeze = "Inicializace tisku je zablokována";
    cz_lang->err_print_finalizing_freeze = "Nesoulad naložených a vyložených archů!";
    cz_lang->err_stacker_full = "Plný vykladač";
    cz_lang->err_reject_bin_full = "Plná výhybka";
    cz_lang->err_unknown_error = "Neznámá chyba!";

    cz_lang->set_lan_label = "Jazyk rozhraní:";
    cz_lang->set_net_iij_tcp_port = "Číslo TCP portu GISu:";
    cz_lang->set_net_iij_ip_addr = "IP adresa GISu:";
    cz_lang->set_net_iij_connection = "Ovládání síťového spojení:";
    cz_lang->set_net_iij_wrong_tcp_port_label = "Chybný formát TCP portu!";
    cz_lang->set_net_iij_wrong_ip_address_label = "Chybný formát IP adresy!";
    cz_lang->set_net_iij_connection_test_label = "IP adresa počítače IIJ:";
    cz_lang->set_net_pci_connection_test_label = "IP adresa počítače PCI:";
    cz_lang->set_net_quadient_connection_test_label = "IP adresa počítače Quadient:";

    cz_lang->file_chooser_btn_browse = "Procházet";
    cz_lang->file_chooser_cancel_btn = "Zrušit";
    cz_lang->file_chooser_select_btn = "Vybrat";
    cz_lang->hot_q_main_fs_title = "Datový hotfolder.";
    cz_lang->hot_q_feedback_fs_title = "Zpětnovazební hotfolder.";
    cz_lang->hot_q_backup_fs_title = "Hotfolder pro zálohování.";
    cz_lang->hot_pci_in_fs_title = "Vstupní kamerový hotfolder.";
    cz_lang->hot_pci_out_fs_title = "Výstupní kamerový hotfolder.";
    cz_lang->hot_gis_fs_title = "Hotfolder pro GIS.";
    cz_lang->hot_report_csv_fs_title = "Reportovací adresář.";

    cz_lang->hot_q_main_fs_label = "Datový hotfolder Quadient:";
    cz_lang->hot_q_feedback_fs_label = "Zpětnovazební hotfolder Quadient:";
    cz_lang->hot_q_backup_fs_label = "Hotfolder pro zálohování Quadient:";
    cz_lang->hot_pci_in_fs_label = "Vstupní kamerový hotfolder:";
    cz_lang->hot_pci_out_fs_label = "Výstupní kamerový hotfolder:";
    cz_lang->hot_gis_fs_label = "Hotfolder pro GIS:";
    cz_lang->hot_report_csv_fs_label = "Adresář pro reporty:";

    cz_lang->rep_csv_state = "Stav ukončení";
    cz_lang->rep_csv_name = "Název-ID jobu";
    cz_lang->rep_csv_rejected_sheets = "Celk. počet vadných archů";
    cz_lang->rep_csv_sheet_number = "Celkový počet archů";
    cz_lang->rep_csv_stamp_number = "Celkový počet kolků";
    cz_lang->rep_csv_date_time = "Datum a čas";

    cz_lang->g_counters_label = "POČÍTADLA STROJE:";
    cz_lang->g_feeder_lbl = "NAKLADAČE:";
    cz_lang->g_status_gis_lbl = "GIS tiskový řadič status:";
    cz_lang->g_status_machine_lbl = "Status stroje:";
    cz_lang->g_cnt_main_feed_lbl = "Naložené archy hlavní nakladač:";
    cz_lang->g_stacker_lbl = "VYKLADAČ:";
    cz_lang->g_reject_bin_lbl = "VÝHYBKA:";
    cz_lang->g_cnt_companion_feed_lbl = "Naložené prokladové archy:";
    cz_lang->g_cnt_rejected_lbl = "Vadné archy:";
    cz_lang->g_cnt_stakced_lbl = "Archy ve vykladači očekávané/aktuální:";
    cz_lang->g_cnt_rejected_seq_lbl = "Sekvence vadných archů:";
    cz_lang->g_cnt_tab_insert_blb = "Nastřelovací proužky:";
    cz_lang->g_job_list_lbl = "Aktuální tisková úloha:";
    cz_lang->g_report_csv_list_lbl = "Historie vytištěných úloh:";

    cz_lang->par_max_stacked_sheet_lbl = "Maximum archů ve vykladači:";
    cz_lang->par_rejected_sheet_seq_lbl = "Maximální sekvence vadných archů:";
    cz_lang->par_sheet_source_lbl = "Nakladač pro prokladový arch:";
    cz_lang->par_print_confirm_lbl = "Potvrzení naložení archu z hlavního nakladače:";
    cz_lang->par_sheet_source_main = "Hlavní nakladač";
    cz_lang->par_sheet_source_companion = "Prokladový nakladač";
    cz_lang->par_machine_mode_lbl = "Pracovní režim dopravníku: ";
    cz_lang->par_mm_setup_comb = "NASTAVENÍ";
    cz_lang->par_mm_inspection_comb = "INSPEKCE";
    cz_lang->par_mm_print_comb = "TISK";
    cz_lang->par_mm_print_inspection_comb = "TISK S INSPEKCÍ";
    cz_lang->par_stacker_status = "Stav vykladače:";
    cz_lang->par_feeder_status = "Stav nakladače:";

    cz_lang->feeder_status_ready = "Připraven";
    cz_lang->feeder_status_ready_to_feed = "Připraven k nakládání";
    cz_lang->feeder_status_feeding = "Nakládání";
    cz_lang->stacker_status_ready = "Připraven";
    cz_lang->stacker_status_ready_to_stack = "Připraven k vykládání";
    cz_lang->stacker_status_stacking = "Vykládání";	
	
    cz_lang->gui_go_to_csv_manage_page_label = "Exportovat csv";

    cz_lang->filter_day = "Den:";
    cz_lang->filter_year = "Rok:";
    cz_lang->filter_month = "Měsíc:";

    cz_lang->csv_manage_return = "Zpět";
    cz_lang->csv_manage_export = "Exportovat";

    cz_lang->to = "Do data:";
    cz_lang->from = "Od data:";

    cz_lang->print_active = "Tiskový modul aktivní";
}

void lang_init_english(lang * en_lang)
{
 /* English labels */
    en_lang->lang_name = "English";
    en_lang->win_title = "Host-BK EN";
    en_lang->gui_job_state = "Actual state";
    en_lang->gui_job_name = "Job name";
    en_lang->gui_job_order = "Job index";
    en_lang->gui_job_sheet_number = "Total sheet number";
    en_lang->gui_job_stamp_number = "Total stamp number";
    en_lang->gui_job_date_time = "Date and time";
    en_lang->print_state_print = "Printing";
    en_lang->print_state_job_error = "Job in error";
    en_lang->print_state_wait = "Waiting for start";
    en_lang->print_state_wait_for_data = "Waiting for data";
    en_lang->set_btn_network = "Network settings";
    en_lang->set_btn_lang_settings = "Language settings";
    en_lang->set_btn_print_param = "Print parameters";
    en_lang->set_btn_hotfolder = "Hotfolder settings";
    en_lang->set_btn_io_vision = "IO vision";
    en_lang->set_btn_back = "Back to control page";
    en_lang->set_btn_back_to_settngs = "Back";

    en_lang->err_no_error = "Ready";
    en_lang->err_cant_save_f_csv = "Can't save feedback csv!";
    en_lang->err_analyze_camera_csv = "Camera csv analyzing error!";
    en_lang->err_cant_load_bkcore_csv = "Can't load control csv!";
    en_lang->err_cant_copy_pdf_to_gis = "Can't send pdf file to GIS!";
    en_lang->err_cant_copy_csv_to_camera = "Can't send csv file to camera!";
    en_lang->err_job_order_mismatch = "Job index order mismatch!";
    en_lang->err_stacker_jam_pile = "Stacker jam pile!";
    en_lang->err_stacker_jam_conveyor = "Stacker jam conveyer!";
    en_lang->err_stacker_bottom = "Stakcer bottom!";
    en_lang->err_stacker_multifunction = "Stacker mulfunction!";
    en_lang->err_stacker_off = "Stacker in off state!";
    en_lang->err_feeder_e_stop = "Feeder E-stop!";
    en_lang->err_feeder_off = "Feeder in off state!";
    en_lang->err_feeder_multifunction = "Feeder mulfunction!";
    en_lang->err_feeder_sheet_missing = "Sheet missing!";
    en_lang->err_feeder_double_sheet = "Double sheet!";
    en_lang->err_machine_e_stop = "Machine E-stop!";
    en_lang->err_reject_bin_jam = "Reject bin jam!";
    en_lang->err_feeder_jam = "Feeder jam!";
    en_lang->err_ti = "Tab insert error!";
    en_lang->err_ta = "Companion sheet feeder error!";
    en_lang->err_counters_mismatch = "Counters mismatch!";
    en_lang->err_low_print_quality = "The maximum number of consecutive sheets has been exceeded!";
    en_lang->err_gis_disconnected = "GIS computer not responding!";
    en_lang->err_csv_sheet_num = "Wrong sheet order in csv file!"; 
    en_lang->err_sheet_feeder_redirection = "Error with feeder redirection for companion sheet!";
    en_lang->err_pci_computer_not_responding = "PCI computer not responding!";
    en_lang->err_iij_computer_not_responding = "IIJ computer not responding!";
    en_lang->err_quadient_computer_not_responding = "Quadient computer not responding!";
    en_lang->err_print_main_freeze = "Feeding of main sheet is blocked";
    en_lang->err_print_companion_freeze = "Feeding of companion sheet is blocked";
    en_lang->err_print_initialization_freeze = "Printing intialization is blocked";
    en_lang->err_print_finalizing_freeze = "Mismatch of feeded and stacked sheets!";
    en_lang->err_stacker_full = "Stacker full!";
    en_lang->err_reject_bin_full = "Reject bin full!";
    en_lang->err_unknown_error = "Unknown error!";

    en_lang->set_lan_label = "Interface language:";
    en_lang->set_net_iij_tcp_port = "TCP port nuber for GIS:";
    en_lang->set_net_iij_ip_addr = "IP address for GIS:";
    en_lang->set_net_iij_connection = "Network connection control:";
    en_lang->set_net_iij_wrong_tcp_port_label = "Wrong TCP port format!";
    en_lang->set_net_iij_wrong_ip_address_label = "Wrong IP address format";
    en_lang->set_net_iij_connection_test_label = "IP address of IIJ computer:";
    en_lang->set_net_pci_connection_test_label = "IP address of PCI computer:";
    en_lang->set_net_quadient_connection_test_label = "IP address of Quadient computer:";

    en_lang->file_chooser_btn_browse = "Browse";
    en_lang->file_chooser_cancel_btn = "Cancel";
    en_lang->file_chooser_select_btn = "Select";
    en_lang->hot_q_main_fs_title = "Data hotfolder";
    en_lang->hot_q_feedback_fs_title = "Feedback hotfolder";
    en_lang->hot_q_backup_fs_title = "Backup hotfolder";
    en_lang->hot_pci_in_fs_title = "Camera input hotfolder";
    en_lang->hot_pci_out_fs_title = "Camera output hotfolder";
    en_lang->hot_gis_fs_title = "GIS hotfolder";
    en_lang->hot_report_csv_fs_title = "Report directory";

    en_lang->hot_q_main_fs_label = "Data hotfolder Quadient:";
    en_lang->hot_q_feedback_fs_label = "Feedback hotfolder Quadient:";
    en_lang->hot_q_backup_fs_label = "Backup hotfolder Quadient:";
    en_lang->hot_pci_in_fs_label = "Camera input hotfolder:";
    en_lang->hot_pci_out_fs_label = "Camera output hotfolder:";
    en_lang->hot_gis_fs_label = "GIS hotfolder:";
    en_lang->hot_report_csv_fs_label = "Report directory:";

    en_lang->rep_csv_state = "Job status";
    en_lang->rep_csv_name = "Job Name-ID";
    en_lang->rep_csv_rejected_sheets = "Total rejected sheet number";
    en_lang->rep_csv_sheet_number = "Total sheet number";
    en_lang->rep_csv_stamp_number = "Total stamp number";
    en_lang->rep_csv_date_time = "Date and time";

    en_lang->g_counters_label = "MACHINE COUNTERS:";
    en_lang->g_feeder_lbl = "FEEDERS:";
    en_lang->g_status_gis_lbl = "GIS print controler status:";
    en_lang->g_status_machine_lbl = "Machine status";
    en_lang->g_cnt_main_feed_lbl = "Feeded sheets main feeder:";
    en_lang->g_stacker_lbl = "STAKCER:";
    en_lang->g_reject_bin_lbl = "Reject bin:";
    en_lang->g_cnt_companion_feed_lbl = "Feeded companion sheets:";
    en_lang->g_cnt_rejected_lbl = "Rejected sheets:";
    en_lang->g_cnt_stakced_lbl = "Stacked sheets expected/current:";
    en_lang->g_cnt_rejected_seq_lbl = "Rejected sheets sequence:";
    en_lang->g_cnt_tab_insert_blb = "Tab inserts:";
    en_lang->g_job_list_lbl = "Current print job:";
    en_lang->g_report_csv_list_lbl = "Printed job history:";

    en_lang->par_max_stacked_sheet_lbl = "Maximum number of sheets in the stecker:";
    en_lang->par_rejected_sheet_seq_lbl = "Maximum number of rejected sheets in sequence:";
    en_lang->par_sheet_source_lbl = "Feeder for companion sheet:";
    en_lang->par_print_confirm_lbl = "Confirmation feeding from main feeder:";
    en_lang->par_sheet_source_main = "Main feeder";
    en_lang->par_sheet_source_companion = "Companion feeder";
    en_lang->par_machine_mode_lbl = "Conveyor work mode:";
    en_lang->par_mm_setup_comb = "SETUP";
    en_lang->par_mm_inspection_comb = "INSPECTION";
    en_lang->par_mm_print_comb = "PRINT";
    en_lang->par_mm_print_inspection_comb = "PRINT INSPECTION";
    en_lang->par_stacker_status = "Stacker status:";
    en_lang->par_feeder_status = "Feeder status:";

    en_lang->feeder_status_ready = "Feeder ready";
    en_lang->feeder_status_ready_to_feed = "Feeder ready to feed";
    en_lang->feeder_status_feeding = "Feeding";
    en_lang->stacker_status_ready = "Stacker ready";
    en_lang->stacker_status_ready_to_stack = "Stacker ready to stack";
    en_lang->stacker_status_stacking = "Stacking";	

    en_lang->gui_go_to_csv_manage_page_label = "Export csv";

    en_lang->filter_day = "Day:";
    en_lang->filter_year = "Year:";
    en_lang->filter_month = "Month:";
	
    en_lang->csv_manage_return = "Back";
    en_lang->csv_manage_export = "Export";

    en_lang->to = "From date:";
    en_lang->from = "To date:";


    en_lang->print_active = "Printer module active";
}

void multi_lang_init()
{
    multi_lang = (lang**) malloc(sizeof(lang*)*lang_num);

    multi_lang[lang_cz] = (lang*) malloc(sizeof(lang));
    multi_lang[lang_en] = (lang*) malloc(sizeof(lang));

    lang_init_czech(multi_lang[lang_cz]);
    lang_init_english(multi_lang[lang_en]);
}

