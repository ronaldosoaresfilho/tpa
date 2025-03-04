document.getElementById("inputForm").addEventListener("submit", async function(event) {
    event.preventDefault();

    let inputField = document.getElementById("userInput");
    let inputText = inputField.value.trim().toLowerCase();

    let response = await fetch("/process", {
        method: "POST",
        headers: { "Content-Type": "application/x-www-form-urlencoded" },
        body: "user_input=" + encodeURIComponent(inputText)
    });

    let result = await response.text();
    let output = document.getElementById("output");
    result = result.replace(/\n/g, "<br>");
    result = result.replace(/\s*\[(\d+)\]/g, (match, n) => `<sup>${n}</sup>`);

    output.innerHTML = result;

    inputField.value = "";
});
