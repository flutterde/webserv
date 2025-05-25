
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
            // Disable the button and show the loading spinner
            uploadButton.disabled = true;
            uploadButton.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Uploading...';
            
            // Create FormData and append the file
            const formData = new FormData();
            formData.append('file', file);

            // Send the file to the server using fetch
            fetch('/uploads', {
                method: 'POST',
                body: formData
            })
            .then(response => response.json()) // Assuming server returns JSON response
            .then(data => {
                // Handle success response
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
            // If no file is selected
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
            // Disable the button and show the loading spinner
            deleteButton.disabled = true;
            deleteButton.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Deleting...';

            // Construct the URL for the DELETE request, using the path entered in the input field
            const deleteUrl = `http://127.0.0.1:8080${path}`;

            // For now, just simulate the DELETE request using a console log
            // (since no backend is available to handle the DELETE request)
            console.log(`Sending DELETE request to: ${deleteUrl}`);

            // Send DELETE request to the server
            fetch(deleteUrl, {
                method: 'DELETE',
            })
            .then(response => {
                // Check if the response status code is in the 2xx range (successful)
                if (response.ok) {
                    deleteButton.disabled = false;
                    deleteButton.innerHTML = '<i class="fas fa-trash-alt"></i> Delete';

                    if (deleteStatus) {
                        deleteStatus.textContent = 'File/Folder deleted successfully!';
                        deleteStatus.className = 'status-message success';
                        deleteStatus.style.display = 'block';
                    }

                    // Clear the input field
                    if (deletePath) deletePath.value = '';
                } else {
                    throw new Error('Error: ' + response.statusText);
                }
            })
            .catch(error => {
                // Handle any errors (network issues or server errors)
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
            // If no path is entered
            deleteStatus.textContent = 'Please enter a file or folder path.';
            deleteStatus.className = 'status-message error';
            deleteStatus.style.display = 'block';
        }
    });
}


