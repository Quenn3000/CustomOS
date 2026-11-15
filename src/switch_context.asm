[bits 32]

; void switch_context(uint32_t* current_esp_ptr, uint32_t* next_esp)
global switch_context
switch_context:
	pushfd ; eflags : 4 octets
	pushad ; les autres : 32 octets

	mov eax, [esp + 40] ; on récupère current_esp_ptr ; 
	cmp eax, 0 ; on vérifie que NULL n'est pas passé en argument
	je .null_arg ; si c'est le cas, on skip le stockage de l'ancien esp
	mov [eax], esp ; on stocke esp à l'adresse de current_esp_ptr

.null_arg
	mov esp, [esp + 44] ; on passe esp à next_esp_ptr (4 = sizeof(uint32_t*))

	popad
	popfd

	ret