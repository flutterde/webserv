
document.addEventListener('DOMContentLoaded', function() {
    if (typeof particlesJS !== 'undefined') {
        particlesJS('particles-js', {
            "particles": {
                "number": {
                    "value": 80,
                    "density": {
                        "enable": true,
                        "value_area": 800
                    }
                },
                "color": {
                    "value": "#6c5ce7"
                },
                "shape": {
                    "type": "circle",
                    "stroke": {
                        "width": 0,
                        "color": "#000000"
                    }
                },
                "opacity": {
                    "value": 0.5,
                    "random": true
                },
                "size": {
                    "value": 3,
                    "random": true
                },
                "line_linked": {
                    "enable": true,
                    "distance": 150,
                    "color": "#6c5ce7",
                    "opacity": 0.4,
                    "width": 1
                },
                "move": {
                    "enable": true,
                    "speed": 2,
                    "direction": "none",
                    "random": true,
                    "straight": false,
                    "out_mode": "out",
                    "bounce": false
                }
            },
            "interactivity": {
                "detect_on": "canvas",
                "events": {
                    "onhover": {
                        "enable": true,
                        "mode": "grab"
                    },
                    "onclick": {
                        "enable": true,
                        "mode": "push"
                    },
                    "resize": true
                },
                "modes": {
                    "grab": {
                        "distance": 140,
                        "line_linked": {
                            "opacity": 1
                        }
                    },
                    "push": {
                        "particles_nb": 4
                    }
                }
            },
            "retina_detect": true
        });
    }


    const revealElements = document.querySelectorAll('.reveal');
    
    const revealOnScroll = () => {
        revealElements.forEach(element => {
            const elementTop = element.getBoundingClientRect().top;
            const elementVisible = 150;
            
            if (elementTop < window.innerHeight - elementVisible) {
                element.classList.add('active');
            }
        });
    };
    
    window.addEventListener('scroll', revealOnScroll);
    revealOnScroll();


    const tabButtons = document.querySelectorAll('.tab-button');
    const tabPanes = document.querySelectorAll('.tab-pane');
    
    tabButtons.forEach(button => {
        button.addEventListener('click', () => {

            tabButtons.forEach(btn => btn.classList.remove('active'));
            

            button.classList.add('active');
            
            const tabId = button.getAttribute('data-tab');
            tabPanes.forEach(pane => {
                pane.classList.remove('active');
                if (pane.id === tabId) {
                    pane.classList.add('active');
                }
            });
        });
    });

    const fileInput = document.getElementById('fileInput');
    const fileName = document.getElementById('fileName');
    
    if (fileInput && fileName) {
        fileInput.addEventListener('change', () => {
            if (fileInput.files.length > 0) {
                fileName.textContent = fileInput.files[0].name;
            } else {
                fileName.textContent = '';
            }
        });
    }
});


const uploadButton = document.getElementById('uploadButton');
if (uploadButton) {
    uploadButton.addEventListener('click', () => {
        const fileInput = document.getElementById('fileInput');
        const uploadStatus = document.getElementById('uploadStatus');
        const fileName = document.getElementById('fileName');
        const file = fileInput?.files[0];

        if (file) {
            uploadButton.disabled = true;
            uploadButton.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Uploading...';

            const formData = new FormData();
            formData.append('file', file);

            fetch('/uploads', {
                method: 'POST',
                body: formData,
                mode: 'no-cors' // This is required for your case where the backend is not modified
            })
            .then(response => {
                console.log("the status Code : ", response.status);
                // Check if the status code is in the 2xx range
                if (response.ok) {
                    // If status is 2xx, we assume success even with an empty body
                    return { success: true };
                } else {
                    // If not, return failure
                    return { success: false };
                }
            })
            .then(data => {
                uploadButton.disabled = false;
                uploadButton.innerHTML = '<i class="fas fa-upload"></i> Upload';

                if (uploadStatus) {
                    if (data.success) {
                        uploadStatus.textContent = 'File uploaded successfully!';
                        uploadStatus.className = 'status-message success';
                    } else {
                        uploadStatus.textContent = 'Error uploading file. Please try again.';
                        uploadStatus.className = 'status-message error';
                    }
                    uploadStatus.style.display = 'block';

                    setTimeout(() => {
                        uploadStatus.style.display = 'none';
                    }, 5000);
                }
                if (fileInput) fileInput.value = '';
                if (fileName) fileName.textContent = '';
            })
            .catch(error => {
                console.error('Error uploading file:', error);
                uploadButton.disabled = false;
                uploadButton.innerHTML = '<i class="fas fa-upload"></i> Upload';

                if (uploadStatus) {
                    uploadStatus.textContent = 'Error uploading file. Please try again.';
                    uploadStatus.className = 'status-message error';
                    uploadStatus.style.display = 'block';

                    setTimeout(() => {
                        uploadStatus.style.display = 'none';
                    }, 5000);
                }
            });

        } else if (uploadStatus) {
            uploadStatus.textContent = 'Please select a file to upload.';
            uploadStatus.className = 'status-message error';
            uploadStatus.style.display = 'block';
        }
    });
}





const deleteButton = document.getElementById('deleteButton');
if (deleteButton) {
    deleteButton.addEventListener('click', () => {
        const deletePath = document.getElementById('deletePath');
        const deleteStatus = document.getElementById('deleteStatus');
        const path = deletePath?.value;

        if (path) {
            deleteButton.disabled = true;
            deleteButton.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Deleting...';
            const deleteUrl = `http://127.0.0.1:5050${path}`;

            console.log(`Sending DELETE request to: ${deleteUrl}`);

            fetch(deleteUrl, {
                method: 'DELETE',
            })
            .then(response => {
                if (response.ok) {
                    deleteButton.disabled = false;
                    deleteButton.innerHTML = '<i class="fas fa-trash-alt"></i> Delete';

                    if (deleteStatus) {
                        deleteStatus.textContent = 'File/Folder deleted successfully!';
                        deleteStatus.className = 'status-message success';
                        deleteStatus.style.display = 'block';
                    }

                    if (deletePath) deletePath.value = '';
                } else {
                    throw new Error('Error: ' + response.statusText);
                }
            })
            .catch(error => {
                console.error('Error:', error);
                deleteButton.disabled = false;
                deleteButton.innerHTML = '<i class="fas fa-trash-alt"></i> Delete';

                if (deleteStatus) {
                    deleteStatus.textContent = 'Error deleting the file/folder. Please try again.';
                    deleteStatus.className = 'status-message error';
                    deleteStatus.style.display = 'block';

                    setTimeout(() => {
                        deleteStatus.style.display = 'none';
                    }, 5000);
                }
            });
        } else if (deleteStatus) {
            deleteStatus.textContent = 'Please enter a file or folder path.';
            deleteStatus.className = 'status-message error';
            deleteStatus.style.display = 'block';
        }
    });
}


