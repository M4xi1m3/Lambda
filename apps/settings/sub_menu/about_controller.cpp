#include "about_controller.h"
#include <assert.h>
#include <cmath>

namespace Settings {

AboutController::AboutController(Responder * parentResponder) :
  GenericSubController(parentResponder),
  m_view(&m_selectableTableView)
{
  for (int i = 0; i < k_totalNumberOfCell; i++) {
    m_cells[i].setMessageFont(KDFont::LargeFont);
    m_cells[i].setAccessoryFont(KDFont::SmallFont);
    m_cells[i].setAccessoryTextColor(Palette::GrayDark);
  }
}

bool AboutController::handleEvent(Ion::Events::Event event) {
  /* We hide here the activation hardware test app: in the menu "about", by
   * clicking on '6' on the last row. */
  if ((event == Ion::Events::Six || event == Ion::Events::LowerT || event == Ion::Events::UpperT) && m_messageTreeModel->label() == I18n::Message::About && selectedRow() == numberOfRows()-1) {
    Container::activeApp()->displayModalViewController(&m_hardwareTestPopUpController, 0.f, 0.f, Metric::ExamPopUpTopMargin, Metric::PopUpRightMargin, Metric::ExamPopUpBottomMargin, Metric::PopUpLeftMargin);
    return true;
  }
  if (event == Ion::Events::OK || event == Ion::Events::EXE) {
    if (selectedRow() == 0) {
      /* When pressing OK on the version cell, the display cycles between
       * Epsilon version number, the commit hash for this build of Epsilon, and
       * the PCB revision number. */
      MessageTableCellWithBuffer * myCell = (MessageTableCellWithBuffer *)m_selectableTableView.selectedCell();
      const char * currentText = myCell->accessoryText();
      if (strcmp(currentText, Ion::patchLevel()) == 0) {
        myCell->setAccessoryText(Ion::pcbVersion());
      } else if (strcmp(currentText, Ion::pcbVersion()) == 0) {
        myCell->setAccessoryText(Ion::softwareVersion());
      } else {
        assert(strcmp(currentText, Ion::softwareVersion()) == 0);
        myCell->setAccessoryText(Ion::patchLevel());
      }
      return true;
    }
    return false;
  }
  return GenericSubController::handleEvent(event);
}

HighlightCell * AboutController::reusableCell(int index, int type) {
  assert(type == 0);
  assert(index >= 0 && index < k_totalNumberOfCell);
  return &m_cells[index];
}

int AboutController::reusableCellCount(int type) {
  assert(type == 0);
  return k_totalNumberOfCell;
}

static char s_SizeBuffer[6];

void AboutController::willDisplayCellForIndex(HighlightCell * cell, int index) {
  GenericSubController::willDisplayCellForIndex(cell, index);
  MessageTableCellWithBuffer * myCell = (MessageTableCellWithBuffer *)cell;
  static const char * messages[] = {
    Ion::softwareVersion(),
    Ion::serialNumber(),
    Ion::fccId(),
    Ion::storageSize(s_SizeBuffer)
  };
  assert(index >= 0 && index < 4);
  myCell->setAccessoryText(messages[index]);
}

}
