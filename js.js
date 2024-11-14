document.getElementById('send-button').addEventListener('click', () => {
    const inputText = document.getElementById('input-box').value;
    document.getElementById('display-box').innerText = inputText;
    document.getElementById('input-box').value = ''; // Clear the input box
});
