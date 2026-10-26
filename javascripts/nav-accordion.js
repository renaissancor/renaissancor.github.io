// Sidebar accordion: opening one section closes its siblings.
document.addEventListener("change", (e) => {
  const t = e.target;
  if (!t.matches("input.md-nav__toggle") || !t.checked) return;
  const item = t.closest(".md-nav__item");
  if (!item || !item.parentElement) return;
  for (const sib of item.parentElement.children) {
    if (sib === item) continue;
    const other = sib.querySelector(":scope > input.md-nav__toggle");
    if (other) other.checked = false;
  }
});
