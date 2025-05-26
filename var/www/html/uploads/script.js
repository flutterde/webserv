function addNewTask() {
	const task = document.getElementById("task");
	const tasks = document.getElementById("tasks");

	if (task.value.trim().length !== 0) {
		const newTask = document.createElement('div');
		newTask.className = "added-task";
		newTask.id = "added-task";

		const newTaskCheckbox = document.createElement("input");
		newTaskCheckbox.type = "checkbox";
		newTaskCheckbox.className = "task-checkbox";
		newTaskCheckbox.id = "task-checkbox";
		newTask.appendChild(newTaskCheckbox);

		const newTaskContent = document.createElement("div");
		newTaskContent.className = "added-task-content";
		newTaskContent.id = "added-task-content";
		newTaskContent.innerText = task.value;
		newTask.appendChild(newTaskContent);

		const newCloseBtn = document.createElement("button");
		newCloseBtn.className = "close-btn";
		newCloseBtn.id = "close-btn";
		newTask.appendChild(newCloseBtn);

		tasks.appendChild(newTask);
		task.value = "";
	}
}

document.getElementById("plus-btn").addEventListener('click', (e) => {
	addNewTask();
});

document.getElementById("task").addEventListener("keypress", (e) => {
	if (e.key == "Enter") {
		addNewTask();
	}
});

document.getElementById('board').addEventListener('click', (e) => {
	if (e.target.classList.contains('close-btn')) {
		e.target.closest('.added-task').remove();
	}
});

document.addEventListener('click', function (event) {
    if (event.target.classList.contains('added-task-content') || ) {
		event.target.classList.toggle('task-checked');
	}
});




// const checkbox = document.querySelector(".tasks");
// console.log(checkbox.childElementCount);